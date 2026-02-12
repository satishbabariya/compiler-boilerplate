// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "testing/base/global_exe_path.h"

#include <optional>
#include <string>

#include "common/check.h"
#include "common/exe_path.h"

static constinit std::optional<std::string> exe_path = {};

namespace MyLang::Testing {

auto GetExePath() -> llvm::StringRef {
  MYLANG_CHECK(
      exe_path,
      "Must not query the executable path until after it has been set!");
  return *exe_path;
}

auto SetExePath(const char* argv_zero) -> void {
  MYLANG_CHECK(!exe_path, "Must not call `SetExePath` more than once!");
  exe_path.emplace(MyLang::FindExecutablePath(argv_zero));
}

}  // namespace MyLang::Testing
