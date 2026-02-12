// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TESTING_BASE_GLOBAL_EXE_PATH_H_
#define MYLANG_TESTING_BASE_GLOBAL_EXE_PATH_H_

#include "llvm/ADT/StringRef.h"

// When using the MyLang `main` function for GoogleTest, we export some extra
// information about the test binary that can be accessed with this header.

namespace MyLang::Testing {

// The executable path of the test binary.
auto GetExePath() -> llvm::StringRef;

// Sets the executable path of a test binary from its `argv[0]`.
//
// This function must only be called once for an execution, and before any
// callers to `GetExePath`. Typically, it is called from within `main`.
auto SetExePath(const char* argv_zero) -> void;

}  // namespace MyLang::Testing

#endif  // MYLANG_TESTING_BASE_GLOBAL_EXE_PATH_H_
