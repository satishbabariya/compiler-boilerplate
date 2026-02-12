// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/install/busybox_info.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <optional>
#include <system_error>

#include "common/check.h"
#include "common/filesystem.h"
#include "llvm/ADT/ScopeExit.h"
#include "llvm/Support/FileSystem.h"

namespace MyLang {
namespace {

using ::testing::Eq;

class BusyboxInfoTest : public ::testing::Test {
 public:
  explicit BusyboxInfoTest()
      : dir_(std::move(*Filesystem::MakeTmpDir())), path_(dir_.path()) {
    // Most tests need the running binary for `MakeBusyboxFile`.
    static int static_for_main_addr;
    running_binary_ = llvm::sys::fs::getMainExecutable("busybox_info_test",
                                                       &static_for_main_addr);
  }

  // Creates a synthetic install tree to test a batch of interactions.
  // Optionally accepts a symlink target for the busybox in the install tree.
  // Returns the input prefix for easy use.
  auto MakeInstallTree(std::filesystem::path prefix,
                       std::optional<std::filesystem::path> busybox_target = {})
      -> std::filesystem::path {
    Filesystem::Dir prefix_dir = *dir_.CreateDirectories(prefix);
    Filesystem::Dir lib_carbon = *prefix_dir.CreateDirectories("lib/mylang");
    if (busybox_target) {
      lib_carbon.Symlink("mylang-busybox", busybox_target->native()).Check();
    } else {
      lib_carbon.Symlink("mylang-busybox", running_binary_).Check();
    }
    Filesystem::Dir llvm_bin = *lib_carbon.CreateDirectories("llvm/bin");
    llvm_bin.Symlink("clang++", "clang").Check();
    llvm_bin.Symlink("clang", "../../mylang-busybox").Check();
    Filesystem::Dir bin = *prefix_dir.OpenDir("bin", Filesystem::CreateNew);
    bin.Symlink("mylang", "../lib/mylang/mylang-busybox").Check();
    return path_ / prefix;
  }

  // The path to the running binary, `busybox_info_test`. This is provided
  // because `GetExecutablePath` can fall back to it.
  std::string running_binary_;

  Filesystem::RemovingDir dir_;
  std::filesystem::path path_;
};

TEST_F(BusyboxInfoTest, Direct) {
  dir_.Symlink("mylang-busybox", running_binary_).Check();

  auto info = GetBusyboxInfo((path_ / "mylang-busybox").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));
}

TEST_F(BusyboxInfoTest, SymlinkInCurrentDirectory) {
  dir_.Symlink("mylang-busybox", running_binary_).Check();
  dir_.Symlink("mylang", "mylang-busybox").Check();

  auto info = GetBusyboxInfo((path_ / "mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));
}

TEST_F(BusyboxInfoTest, SymlinkInCurrentDirectoryWithDot) {
  dir_.Symlink("mylang-busybox", running_binary_).Check();
  dir_.Symlink("mylang", "./mylang-busybox").Check();

  auto info = GetBusyboxInfo((path_ / "mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "./mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));
}

TEST_F(BusyboxInfoTest, ExtraSymlink) {
  dir_.Symlink("mylang-busybox", running_binary_).Check();
  dir_.Symlink("c", "mylang-busybox").Check();
  dir_.Symlink("mylang", "c").Check();

  auto info = GetBusyboxInfo((path_ / "mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));
}

TEST_F(BusyboxInfoTest, OriginalSymlinkNameFormsMode) {
  dir_.Symlink("mylang-busybox", running_binary_).Check();
  dir_.Symlink("mylang", "mylang-busybox").Check();
  dir_.Symlink("clang", "mylang").Check();
  dir_.Symlink("clang++", "clang").Check();

  auto info = GetBusyboxInfo((path_ / "clang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "mylang-busybox"));
  EXPECT_THAT(info->mode, Eq("clang"));

  info = GetBusyboxInfo((path_ / "clang++").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "mylang-busybox"));
  EXPECT_THAT(info->mode, Eq("clang++"));
}

TEST_F(BusyboxInfoTest, BusyboxIsSymlinkToNowhere) {
  dir_.Symlink("mylang-busybox", "nonexistent").Check();

  auto info = GetBusyboxInfo((path_ / "mylang-busybox").c_str());
  ASSERT_FALSE(info.ok());
  EXPECT_THAT(info.error().message(),
              Eq(llvm::formatv("expected mylang-busybox symlink at `{0}`",
                               running_binary_)
                     .str()));
}

TEST_F(BusyboxInfoTest, BusyboxIsWrongFile) {
  // This has the correct name, but it doesn't map back to the running binary
  // and so is ignored.
  dir_.WriteFileFromString("mylang-busybox", "stub").Check();

  auto info = GetBusyboxInfo((path_ / "mylang-busybox").c_str());
  ASSERT_FALSE(info.ok());
  EXPECT_THAT(info.error().message(),
              Eq(llvm::formatv("expected mylang-busybox symlink at `{0}`",
                               running_binary_)
                     .str()));
}

TEST_F(BusyboxInfoTest, RelativeSymlink) {
  Filesystem::Dir d1 = *dir_.OpenDir("dir1", Filesystem::CreateNew);
  d1.Symlink("mylang-busybox", running_binary_).Check();
  Filesystem::Dir d2 = *dir_.OpenDir("dir2", Filesystem::CreateNew);
  d2.Symlink("mylang", "../dir1/mylang-busybox").Check();

  auto info = GetBusyboxInfo((path_ / "dir2/mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "dir2/../dir1/mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));
}

TEST_F(BusyboxInfoTest, AbsoluteSymlink) {
  Filesystem::Dir d1 = *dir_.OpenDir("dir1", Filesystem::CreateNew);
  d1.Symlink("mylang-busybox", running_binary_).Check();
  Filesystem::Dir d2 = *dir_.OpenDir("dir2", Filesystem::CreateNew);
  ASSERT_TRUE(path_.is_absolute());
  d2.Symlink("mylang", (path_ / "dir1/mylang-busybox")).Check();

  auto info = GetBusyboxInfo((path_ / "dir2/mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "dir1/mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));
}

TEST_F(BusyboxInfoTest, NotBusyboxFile) {
  dir_.WriteFileFromString("file", "stub").Check();

  auto info = GetBusyboxInfo((path_ / "file").c_str());
  EXPECT_FALSE(info.ok());
}

TEST_F(BusyboxInfoTest, NotBusyboxSymlink) {
  dir_.WriteFileFromString("file", "stub").Check();
  dir_.Symlink("mylang", "file").Check();

  auto info = GetBusyboxInfo((path_ / "mylang").c_str());
  EXPECT_FALSE(info.ok());
}

TEST_F(BusyboxInfoTest, LayerSymlinksInstallTree) {
  dir_.Symlink("actual-busybox", running_binary_).Check();

  // Create a facsimile of the install prefix with even the busybox as a
  // symlink. Also include potential relative sibling symlinks like `clang++` to
  // `clang`.
  auto prefix = MakeInstallTree("test_prefix", (path_ / "actual-busybox"));

  auto info = GetBusyboxInfo((prefix / "bin/mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(prefix / "bin/../lib/mylang/mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));

  info = GetBusyboxInfo((prefix / "lib/mylang/llvm/bin/clang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path,
              Eq(prefix / "lib/mylang/llvm/bin/../../mylang-busybox"));
  EXPECT_THAT(info->mode, Eq("clang"));

  info = GetBusyboxInfo((prefix / "lib/mylang/llvm/bin/clang++").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path,
              Eq(prefix / "lib/mylang/llvm/bin/../../mylang-busybox"));
  EXPECT_THAT(info->mode, Eq("clang++"));
}

TEST_F(BusyboxInfoTest, StopSearchAtFirstSymlinkWithRelativeBusybox) {
  // Some install of MyLang under `opt`.
  std::filesystem::path opt_prefix = MakeInstallTree("opt");

  // A second install, but with its symlinks pointing into the `opt` tree rather
  // than at its busybox.
  {
    Filesystem::Dir lib_carbon = *dir_.CreateDirectories("lib/mylang");
    lib_carbon.Symlink("mylang-busybox", running_binary_).Check();
    Filesystem::Dir bin = *dir_.OpenDir("bin", Filesystem::CreateNew);
    bin.Symlink("mylang", "../opt/bin/mylang").Check();
    Filesystem::Dir llvm_bin = *lib_carbon.CreateDirectories("llvm/bin");
    llvm_bin.Symlink("clang", (opt_prefix / "lib/mylang/llvm/bin/clang"))
        .Check();
  }

  // Starting from the second install uses the relative busybox rather than
  // traversing the symlink further.
  auto info = GetBusyboxInfo((path_ / "bin/mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "bin/../lib/mylang/mylang-busybox"));
  info = GetBusyboxInfo((path_ / "lib/mylang/llvm/bin/clang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path,
              Eq(path_ / "lib/mylang/llvm/bin/../../mylang-busybox"));
}

TEST_F(BusyboxInfoTest, RejectSymlinkInUnrelatedInstall) {
  // Add two installs of MyLang nested inside each other in a realistic
  // scenario: `/usr` and `/usr/local`.
  MakeInstallTree("usr");
  std::filesystem::path usr_local = MakeInstallTree("usr/local");

  // Now add a stray symlink directly in `.../usr/local` to the local install.
  //
  // This has the interesting property that both of these "work" and find the
  // same busybox but probably wanted to find different ones:
  // - `.../usr/local/../lib/mylang/mylang-busybox`
  // - `.../usr/bin/../lib/mylang/mylang-busybox`
  Filesystem::Dir usr_local_dir = *dir_.OpenDir("usr/local");
  usr_local_dir.Symlink("mylang", "bin/mylang").Check();

  // Check that the busybox doesn't use the relative busybox in this case, and
  // walks the symlink to find the correct installation.
  auto info = GetBusyboxInfo((usr_local / "mylang").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path,
              Eq(usr_local / "bin/../lib/mylang/mylang-busybox"));

  // Ensure this works even with intervening `.` directory components.
  usr_local_dir.Symlink("mylang2", "bin/././mylang").Check();

  // Check that the busybox doesn't use the relative busybox in this case, and
  // walks the symlink to find the correct installation.
  info = GetBusyboxInfo((usr_local / "mylang2").c_str());
  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path,
              Eq(usr_local / "bin/../lib/mylang/mylang-busybox"));
}

TEST_F(BusyboxInfoTest, EnvBinaryPathOverride) {
  // The test should not have this environment variable set.
  ASSERT_THAT(getenv(Argv0OverrideEnv), Eq(nullptr));

  // Set the environment to our actual busybox.
  dir_.Symlink("mylang-busybox", running_binary_).Check();

  setenv(Argv0OverrideEnv, (path_ / "mylang-busybox").c_str(), /*overwrite=*/1);
  auto info = GetBusyboxInfo("/some/nonexistent/path");
  if (getenv(Argv0OverrideEnv)) {
    unsetenv(Argv0OverrideEnv);
    ADD_FAILURE() << "GetBusyboxInfo should unset Argv0OverrideEnv";
  }

  ASSERT_TRUE(info.ok()) << info.error();
  EXPECT_THAT(info->bin_path, Eq(path_ / "mylang-busybox"));
  EXPECT_THAT(info->mode, Eq(std::nullopt));

  // Make sure that we cleaned up the environment afterward.
  EXPECT_THAT(getenv(Argv0OverrideEnv), Eq(nullptr));
}

}  // namespace
}  // namespace MyLang
