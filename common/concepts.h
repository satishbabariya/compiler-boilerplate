// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_COMMON_CONCEPTS_H_
#define MYLANG_COMMON_CONCEPTS_H_

#include <concepts>

namespace MyLang {

// True if `T` is the same as one of `OtherT`.
template <typename T, typename... OtherT>
concept SameAsOneOf = (std::same_as<T, OtherT> || ...);

}  // namespace MyLang

#endif  // MYLANG_COMMON_CONCEPTS_H_
