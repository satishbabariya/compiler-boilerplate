# Compiler Boilerplate

<!--
Part of the Carbon Language project, under the Apache License v2.0 with LLVM
Exceptions. See /LICENSE for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

A clean, modular compiler boilerplate built on LLVM with a fully working Bazel
build system. Derived from the
[Carbon Language](https://github.com/carbon-language/carbon-lang) compiler
infrastructure, this project provides a ready-to-use foundation for building
your own LLVM-based compiled language.

## What You Get

- **Complete Bazel + LLVM build system** -- pre-configured and ready to go
- **Modular compiler pipeline**: Lex -> Parse -> Check -> SemIR -> Lower ->
  CodeGen
- **Rich utility library** (`common/`): hashing, maps, sets, ostream helpers,
  command-line parsing, error handling, and more
- **Testing infrastructure**: file-based tests, fuzzing support, test helpers
- **Language server protocol (LSP)** skeleton
- **Formatter** skeleton
- **Diagnostics system** with source locations and formatting

## Project Structure

```
compiler-boilerplate/
├── common/              # Generic utilities (keep as-is, very reusable)
├── toolchain/
│   ├── base/            # Value stores, index types, shared infrastructure
│   ├── source/          # Source buffer (reading source files)
│   ├── diagnostics/     # Error/warning reporting system
│   ├── lex/             # Lexer skeleton (tokenization)
│   ├── parse/           # Parser skeleton (AST construction)
│   ├── check/           # Semantic analysis skeleton
│   ├── sem_ir/          # Semantic IR (intermediate representation)
│   ├── lower/           # LLVM IR lowering skeleton
│   ├── codegen/         # LLVM code generation (machine code output)
│   ├── driver/          # Compiler driver (CLI entry point)
│   ├── language_server/ # LSP server skeleton
│   ├── format/          # Code formatter skeleton
│   ├── install/         # Installation path handling
│   └── testing/         # Test infrastructure
├── core/                # Language core library stubs
├── examples/            # Example source files
├── testing/             # Shared test infrastructure
├── bazel/               # Bazel rules, toolchains, LLVM integration
├── third_party/         # Third-party dependencies
├── scripts/             # Build and utility scripts
└── docs/                # Documentation
```

## Getting Started

### Prerequisites

You need the following tools installed:

- [Bazel](https://bazel.build/) (via Bazelisk recommended)
- Clang/LLVM toolchain
- LLD (LLVM linker)
- libc++ (LLVM C++ standard library)

On Debian/Ubuntu:

```shell
sudo apt update
sudo apt install clang libc++-dev libc++abi-dev lld
```

On macOS (with Homebrew):

```shell
brew install llvm
```

### Building

```shell
# Build the entire toolchain
bazel build //toolchain

# Build specific stages
bazel build //toolchain/lex/...
bazel build //toolchain/parse/...
bazel build //toolchain/check/...
bazel build //toolchain/lower/...
bazel build //toolchain/driver/...

# Run the compiler
./scripts/run_bazelisk.py run //toolchain -- help

# Run tests
bazel test //toolchain/...
```

## How to Build Your Language

Each compiler stage has stub implementations marked with `TODO` comments. Here
is how to extend each stage:

### 1. Define Your Tokens (`toolchain/lex/`)

Edit `token_kind.def` to add your language's tokens (keywords, operators,
punctuation). Implement the lexer logic in `lex.cpp` to tokenize source text
into a `TokenizedBuffer`.

### 2. Define Your Grammar (`toolchain/parse/`)

Edit `node_kind.def` to define your AST node types. Implement the parser in
`parse.cpp` to build a parse tree from the token stream.

### 3. Implement Semantic Analysis (`toolchain/check/`)

Implement type checking, name resolution, and semantic validation in
`check.cpp`. This stage walks the parse tree and produces SemIR.

### 4. Define Your IR (`toolchain/sem_ir/`)

Edit `inst_kind.def` to define your semantic IR instructions. The SemIR
represents your program in a form suitable for lowering to LLVM IR.

### 5. Lower to LLVM IR (`toolchain/lower/`)

Implement the translation from your SemIR to LLVM IR in `lower.cpp`. This is
where your language semantics get mapped to LLVM's instruction set.

### 6. Code Generation (`toolchain/codegen/`)

The code generation stage is already functional -- it uses LLVM's backend to
produce machine code from LLVM IR. You generally do not need to modify this.

### 7. Add Diagnostics (`toolchain/diagnostics/`)

Add error and warning messages in `kind.def` for each compiler stage. The
diagnostics system supports source locations, fix-it hints, and formatting.

## Architecture

The compiler follows a traditional multi-pass architecture:

```
Source File
    │
    ▼
┌─────────┐
│  Lexer   │  Source text → Token stream
└────┬─────┘
     │
     ▼
┌─────────┐
│  Parser  │  Token stream → Parse tree
└────┬─────┘
     │
     ▼
┌─────────┐
│  Check   │  Parse tree → Semantic IR
└────┬─────┘
     │
     ▼
┌─────────┐
│  Lower   │  Semantic IR → LLVM IR
└────┬─────┘
     │
     ▼
┌─────────┐
│ CodeGen  │  LLVM IR → Object file
└─────────┘
```

## License

This project is licensed under the Apache License v2.0 with LLVM Exceptions.
See [LICENSE](LICENSE) for details.

## Acknowledgments

This boilerplate is derived from the
[Carbon Language](https://github.com/carbon-language/carbon-lang) compiler. The
build infrastructure, utility libraries, and compiler architecture are based on
the Carbon team's excellent work.
