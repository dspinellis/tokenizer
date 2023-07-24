/*-
 * Copyright 2014-2023 Diomidis Spinellis
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <cctype>
#include <string>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "CharSource.h"
#include "RustTokenizer.h"
#include "Token.h"

/*
 * Process a single quote literal, returning its token type
 * This can be CHAR_LITERAL, LIFETIME_LITERAL or LABEL_LITERAL.
 */
token_type
RustTokenizer::get_single_quote_literal()
{
	char c0;
	token_type ret = 0;
	bool starts_with_alnum = false;
	int index = 0;

	if (all_contents)
		sequence_hash.reset();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a character literal");
			return 0;
		}
		if (all_contents)
			sequence_hash.add(c0);

		if (c0 == '\\') {
			// Consume one character after the backslash
			// ... to deal with the '\'' problem
			src.get(c0);
			++index;
			if (all_contents)
				sequence_hash.add(c0);
			continue;
		}

		if (c0 == '\'') {
			ret = Token::CHAR_LITERAL; // '.'
			break;
		}

		if (starts_with_alnum) {
			// Label literals \w+:
			if (c0 == ':') {
				ret = Token::LABEL_LITERAL; // '...:
				break;
			}
			// Lifetime literals \w+\W
			if (!isalnum(c0) && c0 != '_') {
				src.push(c0);
				ret = Token::LIFETIME_LITERAL; // '...
				break;
			}
		}
		if (index == 0 && (isalnum(c0) || c0 == '_'))
			starts_with_alnum = true;

		++index;
	}
	if (all_contents)
		push_token(sequence_hash.get());
	return ret;
}

/*
 * Handle (possibly byte) character and string literals starting with delimiter
 * and possibly prefixed with the specified number of delimiter hashes.
 * Delimiter will be one of " or '.
 * Return true on success false on error (EOF).
 */
bool
RustTokenizer::process_literal(char delimiter, int delimiter_hash_count)
{
	bool saw_delimiter = false;
	int hash_count = 0;
	bool raw = (val == "r" || val == "br");
	char c0;

	if (all_contents)
		sequence_hash.reset();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a literal");
			return false;
		}
		if (all_contents)
			sequence_hash.add(c0);

		if (!raw && c0 == '\\') {
			// Consume one character after the backslash
			src.get(c0);
			if (all_contents)
				sequence_hash.add(c0);
			continue;
		}

		if (c0 == delimiter)
			saw_delimiter = true;
		else if (saw_delimiter) {
			if (c0 == '#')
				++hash_count;
			else {
				hash_count = 0;
				saw_delimiter = false;
			}
		}

		if (saw_delimiter && hash_count == delimiter_hash_count) {
			if (all_contents)
				push_token(sequence_hash.get());
			return true;
		}
	}
}

/*
 * Handle (possibly byte) character and string literals and identifiers
 * prefixed with b, r#*, or br#*
 * c0 is the first non-identifier character.  It will be one of " ' #.
 */
token_type
RustTokenizer::get_rb_prefixed_token(char c0)
{
	int hash_count = 0;

	if (c0 == '#') {
		do {
			if (!src.get(c0)) {
				error("EOF encountered while processing raw prefix");
				return 0;
			}
			++hash_count;
		} while (c0 == '#');
	}

	switch (c0) {
	case '\'':
		if (!process_literal(c0, hash_count))
			return 0;
		if (val == "b" || val == "br")
			return Token::BYTE_LITERAL; // b'.'
		else
			return Token::CHAR_LITERAL; // '.'
		break;
	case '"':
		if (!process_literal(c0, hash_count))
			return 0;
		if (val == "b" || val == "br")
			return Token::BYTE_STRING_LITERAL; // b\"...\"
		else
			return Token::STRING_LITERAL; // \"...\"
		break;
	default:
		val = c0;
		for (;;) {
			src.get(c0);
			if (!isalnum(c0) && c0 != '_')
				break;
			val += c0;
		}
		if (!val.length()) {
			error("Invalid content for raw prefix");
			return 0;
		}

		src.push(c0);
		return symbols.value(val);
	}
}

inline token_type
RustTokenizer::get_immediate_token()
{
	char c0, c1, c2;
	Keyword::IdentifierType key;

	for (;;) {
		if (!src.get(c0))
			return 0;

		if (all_contents) {
			token_type t = rle.add(c0);
			if (t)
				return t;
		}

		switch (c0) {
		case '\n':
			bol.saw_newline();
			break;
		case ' ': case '\t': case '\v': case '\f': case '\r':
			break;
		case '{':
			symbols.enter_scope();
			nesting.saw_open_brace();
			return static_cast<token_type>(c0);
		case '}':
			symbols.exit_scope();
			nesting.saw_close_brace();
			return static_cast<token_type>(c0);
		/*
		 * Double character C tokens with more than 2 different outcomes
		 * (e.g. -, -=, ->)
		 */
		case '-':
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::MINUS_EQUAL; // -=
			case '>':
				return Token::RIGHT_SLIM_ARROW; // ->
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '&':
			src.get(c1);
			switch (c1) {
			case '&':
				return Token::BOOLEAN_AND; // &&
			case '=':
				return Token::AND_EQUAL; // &=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '|':
			src.get(c1);
			switch (c1) {
			case '|':
				return Token::BOOLEAN_OR; // ||
			case '=':
				return Token::OR_EQUAL; // |=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '=':
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::EQUAL; // ==
			case '>':
				return Token::RIGHT_FAT_ARROW; // =>
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		/* Simple single/double character tokens (e.g. !, !=) */
		case '!':
			src.get(c1);
			if (c1 == '=')
				return Token::NOT_EQUAL; // !=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '+':
			src.get(c1);
			if (c1 == '=')
				return Token::PLUS_EQUAL; // +=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '%':
			src.get(c1);
			if (c1 == '=')
				return Token::MOD_EQUAL; // %=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '*':
			src.get(c1);
			if (c1 == '=')
				return Token::TIMES_EQUAL; // *=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case ':':
			src.get(c1);
			if (c1 == ':')
				return Token::DOUBLE_COLON; // ::
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '^':
			src.get(c1);
			if (c1 == '=')
				return Token::XOR_EQUAL; // ^=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		/* Operators starting with < or > */
		case '>':
			// class might have been used as a template argument
			nesting.unsaw_class();
			src.get(c1);
			switch (c1) {
			case '=':				/* >= */
				return Token::GREATER_EQUAL; // >=
			case '>':
				src.get(c1);
				if (c1 == '=')			/* >>= */
					return Token::RSHIFT_EQUAL; // >>=
				else {			/* << */
					src.push(c1);
					return Token::RSHIFT; // >>
				}
			default:				/* > */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '<':
			src.get(c1);
			switch (c1) {
			case '=':				/* <= */
				return Token::LESS_EQUAL; // <=
			case '<':
				src.get(c1);
				if (c1 == '=')			/* <<= */
					return Token::LSHIFT_EQUAL; // <<=
				else {			/* << */
					src.push(c1);
					return Token::LSHIFT; // <<
				}
			default:				/* < */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		/* Comments and / operators */
		case '/':
			src.get(c1);
			switch (c1) {
			case '=':				/* /= */
				return Token::DIV_EQUAL; // /=
			case '*':				/* Block comment */
				return get_block_comment_token();
			case '/':				/* Line comment */
				return get_line_comment_token();
			default:				/* / */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		case '.':	/* . .. ..= ... */
			src.get(c1);
			switch (c1) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				val = std::string(".") + (char)(c1);
				return get_number_token(val);
			case '.':  /* .. */
				src.get(c2);
				switch (c2) {
				case '.':
					return Token::ELIPSIS; // ...
				case '=':
					return Token::DOT_DOT_EQUAL; // ..=
				default:
					src.push(c2);
					return Token::DOT_DOT; // ..
				}
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
		/* XXX Can also be non-ASCII */
		case '_': case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
		case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
		case 'r': case 's': case 't': case 'u': case 'v': case 'w':
		case 'x': case 'y': case 'z': case 'A': case 'B': case 'C':
		case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O':
		case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
			val = c0;
			for (;;) {
				src.get(c0);
				if (!isalnum(c0) && c0 != '_')
					break;
				val += c0;
			}
			if ((c0 == '#' || c0 == '\'' || c0 == '"')
			    && (val == "r" || val == "b" || val == "br"))
				return get_rb_prefixed_token(c0);

			// Single _ has special meaning, so return it as a token
			if (val == "_")
				return static_cast<token_type>('_');

			src.push(c0);
			key = rust_keyword.identifier_type(val);
			if (key == Keyword::FIRST_IDENTIFIER)
				return symbols.value(val);
			return key;
		case '\'':
			return get_single_quote_literal();
		case '"':
			if (process_string_literal())
				return Token::STRING_LITERAL; // \"...\"
			else
				return 0;
		/* Various numbers */
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			val = c0;
			return get_number_token(val);
		default:
			return static_cast<token_type>(c0);
		}
	}
}

RustTokenizer::~RustTokenizer()
{
}
