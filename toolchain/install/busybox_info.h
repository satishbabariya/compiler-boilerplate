// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_INSTALL_BUSYBOX_INFO_H_
#define MYLANG_TOOLCHAIN_INSTALL_BUSYBOX_INFO_H_

#include <filesystem>
#include <optional>
#include <string>

#include "common/error.h"

namespace MyLang {

// An optional override of argv0, particularly used by `//toolchain/mylang` to
// get desired behavior without further special-casing.
inline constexpr const char* Argv0OverrideEnv = "MYLANG_ARGV0_OVERRIDE";

struct BusyboxInfo {
  // The path to `mylang-busybox`.
  std::filesystem::path bin_path;
  // The mode, such as `mylang` or `clang`.
  std::optional<std::string> mode;
};

// Returns the busybox information, given argv[0].
//
// Extracts the desired mode for the busybox from the initial command name.
//
// Checks if the path in argv0 is an executable in a valid MyLang install, or a
// symlink to such an executable, and sets `bin_path` to the path of
// `lib/mylang/mylang-busybox` within that install.
//
// If unable to locate a plausible busybox binary, returns an error instead.
auto GetBusyboxInfo(const char* argv0) -> ErrorOr<BusyboxInfo>;

}  // namespace MyLang

#endif  // MYLANG_TOOLCHAIN_INSTALL_BUSYBOX_INFO_H_
