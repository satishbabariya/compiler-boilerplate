// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_SEM_IR_INST_CATEGORIES_H_
#define MYLANG_TOOLCHAIN_SEM_IR_INST_CATEGORIES_H_

#include "toolchain/sem_ir/ids.h"
#include "toolchain/sem_ir/inst_kind.h"
#include "toolchain/sem_ir/typed_insts.h"

// An inst category is a set of inst kinds that can be treated polymorphically.
// Each inst category is represented by a C++ type, just like an inst kind,
// which can losslessly represent any inst in the category.
//
// TODO: Add your language's instruction categories here.
// See MyLang compiler for reference implementation patterns.

namespace MyLang::SemIR {

// Declares a category consisting of `TypedInsts...`.
template <typename... TypedInsts>
struct CategoryOf {
  static constexpr InstKind Kinds[] = {TypedInsts::Kind...};
};

// Common representation for all kinds of `Branch*` node.
struct AnyBranch {
  using CategoryInfo = CategoryOf<Branch, BranchIf, BranchWithArg>;

  InstKind kind;
  LabelId target_id;
  AnyRawId arg1;
};

}  // namespace MyLang::SemIR

#endif  // MYLANG_TOOLCHAIN_SEM_IR_INST_CATEGORIES_H_
