// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/diagnostics/kind.h"  // IWYU pragma: keep

namespace MyLang::Diagnostics {

MYLANG_DEFINE_ENUM_CLASS_NAMES(Kind) {
#define MYLANG_DIAGNOSTIC_KIND(Name) MYLANG_ENUM_CLASS_NAME_STRING(Name)
#include "toolchain/diagnostics/kind.def"
};

}  // namespace MyLang::Diagnostics
