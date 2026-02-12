// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_BASE_VALUE_STORE_TYPES_H_
#define MYLANG_TOOLCHAIN_BASE_VALUE_STORE_TYPES_H_

#include <concepts>
#include <type_traits>

#include "llvm/ADT/StringRef.h"

namespace MyLang {

// Common calculation for ValueStore types.
template <typename ValueT>
class ValueStoreTypes {
 public:
  using ValueType = std::remove_cvref_t<ValueT>;

  // Typically we want to use `ValueType&` and `const ValueType& to avoid
  // copies, but when the value type is a `StringRef`, we assume external
  // storage for the string data and both our value type and ref type will be
  // `StringRef`. This will preclude mutation of the string data.
  using RefType = std::conditional_t<std::same_as<llvm::StringRef, ValueType>,
                                     llvm::StringRef, ValueType&>;
  using ConstRefType =
      std::conditional_t<std::same_as<llvm::StringRef, ValueType>,
                         llvm::StringRef, const ValueType&>;
};

}  // namespace MyLang

#endif  // MYLANG_TOOLCHAIN_BASE_VALUE_STORE_TYPES_H_
