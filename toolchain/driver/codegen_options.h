// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_DRIVER_CODEGEN_OPTIONS_H_
#define MYLANG_TOOLCHAIN_DRIVER_CODEGEN_OPTIONS_H_

#include "common/command_line.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/TargetParser/Host.h"

namespace MyLang {

// Shared codegen-related options.
//
// See the implementation of `Build` for documentation on members.
struct CodegenOptions {
  auto Build(CommandLine::CommandBuilder& b) -> void;

  std::string host = llvm::sys::getDefaultTargetTriple();
  llvm::StringRef target;
};

}  // namespace MyLang

#endif  // MYLANG_TOOLCHAIN_DRIVER_CODEGEN_OPTIONS_H_
