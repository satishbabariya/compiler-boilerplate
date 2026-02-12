// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/lex/token_kind.h"

namespace MyLang::Lex {

MYLANG_DEFINE_ENUM_CLASS_NAMES(TokenKind) {
#define MYLANG_TOKEN(TokenName) MYLANG_ENUM_CLASS_NAME_STRING(TokenName)
#include "toolchain/lex/token_kind.def"
};

constexpr bool TokenKind::IsSymbol[] = {
#define MYLANG_TOKEN(TokenName) false,
#define MYLANG_SYMBOL_TOKEN(TokenName, Spelling) true,
#include "toolchain/lex/token_kind.def"
};

constexpr bool TokenKind::IsGroupingSymbol[] = {
#define MYLANG_TOKEN(TokenName) false,
#define MYLANG_OPENING_GROUP_SYMBOL_TOKEN(TokenName, Spelling, ClosingName) \
  true,
#define MYLANG_CLOSING_GROUP_SYMBOL_TOKEN(TokenName, Spelling, OpeningName) \
  true,
#include "toolchain/lex/token_kind.def"
};

constexpr bool TokenKind::IsOpeningSymbol[] = {
#define MYLANG_TOKEN(TokenName) false,
#define MYLANG_OPENING_GROUP_SYMBOL_TOKEN(TokenName, Spelling, ClosingName) \
  true,
#include "toolchain/lex/token_kind.def"
};

constexpr TokenKind TokenKind::ClosingSymbol[] = {
#define MYLANG_TOKEN(TokenName) Error,
#define MYLANG_OPENING_GROUP_SYMBOL_TOKEN(TokenName, Spelling, ClosingName) \
  ClosingName,
#include "toolchain/lex/token_kind.def"
};

constexpr bool TokenKind::IsClosingSymbol[] = {
#define MYLANG_TOKEN(TokenName) false,
#define MYLANG_CLOSING_GROUP_SYMBOL_TOKEN(TokenName, Spelling, OpeningName) \
  true,
#include "toolchain/lex/token_kind.def"
};

constexpr TokenKind TokenKind::OpeningSymbol[] = {
#define MYLANG_TOKEN(TokenName) Error,
#define MYLANG_CLOSING_GROUP_SYMBOL_TOKEN(TokenName, Spelling, OpeningName) \
  OpeningName,
#include "toolchain/lex/token_kind.def"
};

constexpr bool TokenKind::IsOneCharSymbol[] = {
#define MYLANG_TOKEN(TokenName) false,
#define MYLANG_ONE_CHAR_SYMBOL_TOKEN(TokenName, Spelling) true,
#include "toolchain/lex/token_kind.def"
};

constexpr bool TokenKind::IsKeyword[] = {
#define MYLANG_TOKEN(TokenName) false,
#define MYLANG_KEYWORD_TOKEN(TokenName, Spelling) true,
#include "toolchain/lex/token_kind.def"
};

constexpr llvm::StringLiteral TokenKind::FixedSpelling[] = {
#define MYLANG_TOKEN(TokenName) "",
#define MYLANG_SYMBOL_TOKEN(TokenName, Spelling) Spelling,
#define MYLANG_KEYWORD_TOKEN(TokenName, Spelling) Spelling,
#include "toolchain/lex/token_kind.def"
};

constexpr int8_t TokenKind::ExpectedParseTreeSize[] = {
#define MYLANG_TOKEN(Name) 1,
#define MYLANG_TOKEN_WITH_VIRTUAL_NODE(size) 2,
#include "toolchain/lex/token_kind.def"
};

}  // namespace MyLang::Lex
