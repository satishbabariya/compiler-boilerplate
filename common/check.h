// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_COMMON_CHECK_H_
#define MYLANG_COMMON_CHECK_H_

#include "common/check_internal.h"

namespace MyLang {

// Checks the given condition, and if it's false, prints a stack, streams the
// error message, then exits. This should be used for unexpected errors, such as
// a bug in the application.
//
// For example:
//   MYLANG_CHECK(is_valid, "Data is not valid!");
//
// The condition must be parenthesized if it contains top-level commas, for
// example in a template argument list:
//   MYLANG_CHECK((inst.IsOneOf<Call, TupleLiteral>()),
//                "Unexpected inst {0}", inst);
#define MYLANG_CHECK(condition, ...)         \
  MYLANG_INTERNAL_CHECK_CONDITION(condition) \
  ? (void)0 : MYLANG_INTERNAL_CHECK(condition __VA_OPT__(, ) __VA_ARGS__)

// DCHECK calls CHECK in debug mode, and does nothing otherwise.
#ifndef NDEBUG
#define MYLANG_DCHECK(condition, ...) \
  MYLANG_CHECK(condition __VA_OPT__(, ) __VA_ARGS__)
#else
// When in a debug build we want to preserve as much as we can of how the
// parameters are used, other than making them be trivially in dead code and
// eliminated by the optimizer. As a consequence we preserve the condition but
// prefix it with a short-circuit operator, and we still emit the (dead) call to
// the check implementation. But we use a special implementation that reduces
// the compile time cost.
#define MYLANG_DCHECK(condition, ...)                  \
  (true || MYLANG_INTERNAL_CHECK_CONDITION(condition)) \
      ? (void)0                                        \
      : MYLANG_INTERNAL_DEAD_DCHECK(condition __VA_OPT__(, ) __VA_ARGS__)
#endif

// This is similar to CHECK, but is unconditional. Writing
// `MYLANG_FATAL("message")` is clearer than `MYLANG_CHECK(false, "message")
// because it avoids confusion about control flow.
//
// For example:
//   MYLANG_FATAL("Unreachable!");
#define MYLANG_FATAL(...) MYLANG_INTERNAL_FATAL(__VA_ARGS__)

}  // namespace MyLang

#endif  // MYLANG_COMMON_CHECK_H_
