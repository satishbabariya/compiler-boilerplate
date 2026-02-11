// Part of the Carbon Language project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/lex/lex.h"

#include "common/check.h"
#include "common/vlog.h"
#include "toolchain/base/shared_value_stores.h"
#include "toolchain/lex/character_set.h"
#include "toolchain/lex/token_index.h"
#include "toolchain/lex/token_info.h"
#include "toolchain/lex/token_kind.h"
#include "toolchain/lex/tokenized_buffer.h"

namespace Carbon::Lex {

auto Lex(SharedValueStores& value_stores, SourceBuffer& source,
         LexOptions options) -> TokenizedBuffer {
  // TODO: Implement your language's lexer here.
  // This stage should tokenize source text into a TokenizedBuffer.
  //
  // Typical responsibilities:
  // - Skip whitespace and comments
  // - Recognize keywords, identifiers, literals
  // - Match symbols and operators
  // - Track line/column information
  // - Report lexical errors via diagnostics
  //
  // See the Carbon Language compiler for reference implementation patterns.

  auto* consumer =
      options.consumer ? options.consumer : &Diagnostics::ConsoleConsumer();
  TokenizedBuffer buffer(value_stores, source);

  // Add the file boundary tokens that the parser expects.
  buffer.AddToken(TokenInfo(TokenKind::FileStart,
                            /*has_leading_space=*/false, /*byte_offset=*/0));
  buffer.AddToken(
      TokenInfo(TokenKind::FileEnd, /*has_leading_space=*/true,
                /*byte_offset=*/static_cast<int32_t>(source.text().size())));

  if (options.vlog_stream || options.dump_stream) {
    consumer->Flush();
  }
  CARBON_VLOG_TO(options.vlog_stream, "*** Lex::TokenizedBuffer ***\n{0}",
                 buffer);
  if (options.dump_stream) {
    buffer.Print(*options.dump_stream, options.omit_file_boundary_tokens);
  }
  return buffer;
}

}  // namespace Carbon::Lex
