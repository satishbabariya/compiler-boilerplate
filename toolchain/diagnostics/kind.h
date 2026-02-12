// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_DIAGNOSTICS_KIND_H_
#define MYLANG_TOOLCHAIN_DIAGNOSTICS_KIND_H_

#include <cstdint>

#include "common/enum_base.h"

namespace MyLang::Diagnostics {

// Although this currently fits into int8_t, it shouldn't be expected to
// long-term.
MYLANG_DEFINE_RAW_ENUM_CLASS(Kind, uint16_t) {
#define MYLANG_DIAGNOSTIC_KIND(Name) MYLANG_RAW_ENUM_ENUMERATOR(Name)
#include "toolchain/diagnostics/kind.def"
};

// An enumeration of all diagnostics provided by the toolchain. Diagnostics must
// be added to kind.def, and defined locally to where they're used using the
// `DIAGNOSTIC` macro in emitter.h.
//
// Diagnostic definitions are decentralized because placing all diagnostic
// definitions centrally is expected to create a compilation bottleneck
// long-term, and we also see value to keeping diagnostic format strings close
// to the consuming code.
class Kind : public MYLANG_ENUM_BASE(Kind) {
 public:
#define MYLANG_DIAGNOSTIC_KIND(Name) MYLANG_ENUM_CONSTANT_DECL(Name)
#include "toolchain/diagnostics/kind.def"
};

#define MYLANG_DIAGNOSTIC_KIND(Name) MYLANG_ENUM_CONSTANT_DEFINITION(Kind, Name)
#include "toolchain/diagnostics/kind.def"

// We expect Kind to fit into 2 bits.
static_assert(sizeof(Kind) == 2, "Kind includes padding!");

}  // namespace MyLang::Diagnostics

#endif  // MYLANG_TOOLCHAIN_DIAGNOSTICS_KIND_H_
