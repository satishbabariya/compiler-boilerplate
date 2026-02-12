// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TESTING_FILE_TEST_MANIFEST_H_
#define MYLANG_TESTING_FILE_TEST_MANIFEST_H_

#include <string>

#include "llvm/ADT/SmallVector.h"

namespace MyLang::Testing {

// Returns the manifest path, which is provided by rules.bzl and manifest.cpp.
// This is exposed separately so that the explorer sharding approach can use a
// different implementation.
auto GetFileTestManifest() -> llvm::SmallVector<std::string>;

}  // namespace MyLang::Testing

#endif  // MYLANG_TESTING_FILE_TEST_MANIFEST_H_
