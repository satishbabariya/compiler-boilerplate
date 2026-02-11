# Part of the Carbon Language project, under the Apache License v2.0 with LLVM
# Exceptions. See /LICENSE for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

load("//bazel/cc_rules:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "check",
    srcs = ["check.cpp"],
    hdrs = ["check.h"],
    deps = [
        "//common:ostream",
        "//common:vlog",
        "//toolchain/base:shared_value_stores",
        "//toolchain/base:timings",
        "//toolchain/diagnostics:emitter",
        "//toolchain/parse:tree",
        "//toolchain/sem_ir:file",
        "//toolchain/sem_ir:typed_insts",
        "@llvm-project//clang:frontend",
        "@llvm-project//llvm:Support",
    ],
)
