// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_BASE_INSTALL_PATHS_TEST_HELPERS_H_
#define MYLANG_TOOLCHAIN_BASE_INSTALL_PATHS_TEST_HELPERS_H_

#include "llvm/Support/VirtualFileSystem.h"
#include "toolchain/base/install_paths.h"

namespace MyLang::Testing {

// Prepares the VFS with prelude files from the real filesystem.
auto AddPreludeFilesToVfs(
    const InstallPaths& install_paths,
    llvm::IntrusiveRefCntPtr<llvm::vfs::InMemoryFileSystem>& vfs) -> void;

}  // namespace MyLang::Testing

#endif  // MYLANG_TOOLCHAIN_BASE_INSTALL_PATHS_TEST_HELPERS_H_
