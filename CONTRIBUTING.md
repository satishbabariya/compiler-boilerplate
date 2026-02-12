# Contributing

<!--
Part of the compiler boilerplate, under the Apache License v2.0 with LLVM
Exceptions. See /LICENSE for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

## Overview

This is a compiler boilerplate project. It provides a clean, modular foundation
for building an LLVM-based compiler with Bazel.

## Getting started

1. Clone the repository
2. Ensure you have Bazel installed (see `.bazelversion` for the required version)
3. Build the toolchain: `bazel build //toolchain/...`

## Project structure

The compiler pipeline has several stages, each in its own directory under
`toolchain/`:

- **`toolchain/lex/`** - Lexer: tokenizes source code
- **`toolchain/parse/`** - Parser: builds a parse tree from tokens
- **`toolchain/check/`** - Checker: semantic analysis and SemIR construction
- **`toolchain/sem_ir/`** - Semantic IR: the intermediate representation
- **`toolchain/lower/`** - Lowerer: converts SemIR to LLVM IR
- **`toolchain/codegen/`** - Code generator: emits machine code via LLVM
- **`toolchain/driver/`** - Driver: command-line interface tying everything together

Supporting infrastructure:

- **`toolchain/base/`** - Shared utilities and data structures
- **`toolchain/diagnostics/`** - Error and warning reporting
- **`toolchain/source/`** - Source file handling
- **`common/`** - General-purpose utilities

## How to extend

Each stage has `// TODO:` comments indicating where to add your language's
implementation. Start by:

1. Defining your token kinds in `toolchain/lex/token_kind.def`
2. Defining your parse node kinds in `toolchain/parse/node_kind.def`
3. Implementing your lexer in `toolchain/lex/lex.cpp`
4. Implementing your parser in `toolchain/parse/parse.cpp`
5. Defining your SemIR instructions in `toolchain/sem_ir/inst_kind.def`
6. Implementing semantic checking in `toolchain/check/check.cpp`
7. Implementing lowering to LLVM IR in `toolchain/lower/lower.cpp`

## Code style

This project follows the Google C++ Style Guide. See See `.clang-format` and `.clang-tidy` for automated formatting
and linting configuration.
