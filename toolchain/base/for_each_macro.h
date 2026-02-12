// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_TOOLCHAIN_BASE_FOR_EACH_MACRO_H_
#define MYLANG_TOOLCHAIN_BASE_FOR_EACH_MACRO_H_

/// MYLANG_FOR_EACH() will apply `macro` to each argument in the variadic
/// argument list, putting the output of `sep()` between each one.
///
/// The `sep` should be a function macro that returns a separator. Premade
/// separataors are provided as MYLANG_FOR_EACH_XYZ() macros.
#define MYLANG_FOR_EACH(macro, sep, ...)      \
  __VA_OPT__(MYLANG_INTERNAL_FOR_EACH_EXPAND( \
      MYLANG_INTERNAL_FOR_EACH(macro, sep, __VA_ARGS__)))

#define MYLANG_FOR_EACH_COMMA() ,
#define MYLANG_FOR_EACH_SEMI() ;
#define MYLANG_FOR_EACH_CONCAT()

// Internal helpers

#define MYLANG_INTERNAL_FOR_EACH(macro, sep, a1, ...)                 \
  macro(a1) __VA_OPT__(sep()) __VA_OPT__(                             \
      MYLANG_INTERNAL_FOR_EACH_AGAIN MYLANG_INTERNAL_FOR_EACH_PARENS( \
          macro, sep, __VA_ARGS__))
#define MYLANG_INTERNAL_FOR_EACH_PARENS ()
#define MYLANG_INTERNAL_FOR_EACH_AGAIN() MYLANG_INTERNAL_FOR_EACH

#define MYLANG_INTERNAL_FOR_EACH_EXPAND(...)                             \
  MYLANG_INTERNAL_FOR_EACH_EXPAND1(                                      \
      MYLANG_INTERNAL_FOR_EACH_EXPAND1(MYLANG_INTERNAL_FOR_EACH_EXPAND1( \
          MYLANG_INTERNAL_FOR_EACH_EXPAND1(__VA_ARGS__))))
#define MYLANG_INTERNAL_FOR_EACH_EXPAND1(...)                            \
  MYLANG_INTERNAL_FOR_EACH_EXPAND2(                                      \
      MYLANG_INTERNAL_FOR_EACH_EXPAND2(MYLANG_INTERNAL_FOR_EACH_EXPAND2( \
          MYLANG_INTERNAL_FOR_EACH_EXPAND2(__VA_ARGS__))))
#define MYLANG_INTERNAL_FOR_EACH_EXPAND2(...)                            \
  MYLANG_INTERNAL_FOR_EACH_EXPAND3(                                      \
      MYLANG_INTERNAL_FOR_EACH_EXPAND3(MYLANG_INTERNAL_FOR_EACH_EXPAND3( \
          MYLANG_INTERNAL_FOR_EACH_EXPAND3(__VA_ARGS__))))
#define MYLANG_INTERNAL_FOR_EACH_EXPAND3(...) __VA_ARGS__

#endif  // MYLANG_TOOLCHAIN_BASE_FOR_EACH_MACRO_H_
