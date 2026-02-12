// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "common/check.h"

#include <gtest/gtest.h>

#include <string>

namespace MyLang {
namespace {

// Non-constexpr functions that always return true and false, to bypass constant
// condition checking.
auto AlwaysTrue() -> bool { return true; }
auto AlwaysFalse() -> bool { return false; }

TEST(CheckTest, CheckTrue) { MYLANG_CHECK(AlwaysTrue()); }

TEST(CheckTest, CheckFalse) {
  ASSERT_DEATH(
      { MYLANG_CHECK(AlwaysFalse()); },
      R"(
CHECK failure at common/check_test.cpp:\d+: AlwaysFalse\(\)
)");
}

TEST(CheckTest, CheckFalseHasStackDump) {
  ASSERT_DEATH({ MYLANG_CHECK(AlwaysFalse()); }, "\nStack dump:\n");
}

TEST(CheckTest, CheckTrueCallbackNotUsed) {
  bool called = false;
  auto callback = [&]() {
    called = true;
    return "called";
  };
  MYLANG_CHECK(AlwaysTrue(), "{0}", callback());
  EXPECT_FALSE(called);
}

TEST(CheckTest, CheckFalseMessage) {
  ASSERT_DEATH(
      { MYLANG_CHECK(AlwaysFalse(), "msg"); },
      R"(
CHECK failure at common/check_test.cpp:.+: AlwaysFalse\(\): msg
)");
}

TEST(CheckTest, CheckFalseFormattedMessage) {
  const char msg[] = "msg";
  std::string str = "str";
  int i = 1;
  ASSERT_DEATH(
      { MYLANG_CHECK(AlwaysFalse(), "{0} {1} {2} {3}", msg, str, i, 0); },
      R"(
CHECK failure at common/check_test.cpp:.+: AlwaysFalse\(\): msg str 1 0
)");
}

TEST(CheckTest, CheckOutputForms) {
  const char msg[] = "msg";
  std::string str = "str";
  int i = 1;
  MYLANG_CHECK(AlwaysTrue(), "{0} {1} {2} {3}", msg, str, i, 0);
}

TEST(CheckTest, Fatal) {
  ASSERT_DEATH(
      { MYLANG_FATAL("msg"); },
      "\nFATAL failure at common/check_test.cpp:.+: msg\n");
}

TEST(CheckTest, FatalHasStackDump) {
  ASSERT_DEATH({ MYLANG_FATAL("msg"); }, "\nStack dump:\n");
}

auto FatalNoReturnRequired() -> int { MYLANG_FATAL("msg"); }

TEST(ErrorTest, FatalNoReturnRequired) {
  ASSERT_DEATH(
      { FatalNoReturnRequired(); },
      "\nFATAL failure at common/check_test.cpp:.+: msg\n");
}

// Detects whether `MYLANG_CHECK(F())` compiles.
template <auto F>
concept CheckCompilesWithCondition = requires { MYLANG_CHECK(F()); };

TEST(CheckTest, CheckConstantCondition) {
  EXPECT_TRUE(CheckCompilesWithCondition<[] { return AlwaysTrue(); }>);
  EXPECT_TRUE(CheckCompilesWithCondition<[] { return AlwaysFalse(); }>);
  EXPECT_FALSE(CheckCompilesWithCondition<[] { return true; }>);
  EXPECT_FALSE(CheckCompilesWithCondition<[] { return false; }>);
}

}  // namespace
}  // namespace MyLang
