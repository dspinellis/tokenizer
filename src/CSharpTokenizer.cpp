/*-
 * Copyright 2014-2018 Diomidis Spinellis
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
#include "CSharpTokenizer.h"
#include "CSharpToken.h"

inline int
CSharpTokenizer::get_token_real()
{
	char c0, c1, c2;
	CSharpKeyword::IdentifierType key;

	for (;;) {
		if (!src.get(c0))
			return 0;

		switch (c0) {
		/*
		 * Single character C operators and punctuators
		 * ANSI 3.1.5 p. 32 and 3.1.6 p. 33
		 */
		case '\n':
			break;
		case ' ': case '\t': case '\v': case '\f': case '\r':
			break;
		case '{':
			bol.saw_non_space();
			symbols.enter_scope();
			nesting.saw_open_brace();
			return (int)c0;
		case '}':
			bol.saw_non_space();
			symbols.exit_scope();
			nesting.saw_close_brace();
			return (int)c0;
		case ';':
			bol.saw_non_space();
			return (int)c0;
		/*
		 * Double character C tokens with more than 2 different outcomes
		 * (e.g. +, +=, ++)
		 */
		case '+':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '+':
				return CSharpToken::PLUS_PLUS; // ++
			case '=':
				return CSharpToken::PLUS_EQUAL; // +=
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case '-':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '-':
				return CSharpToken::MINUS_MINUS; // --
			case '=':
				return CSharpToken::MINUS_EQUAL; // -=
			case '>':
				return CSharpToken::MEMBER_PTR; // ->
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case '&':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '&':
				return CSharpToken::BOOLEAN_AND; // &&
			case '=':
				return CSharpToken::AND_EQUAL; // &=
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case '|':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '|':
				return CSharpToken::BOOLEAN_OR; // ||
			case '=':
				return CSharpToken::OR_EQUAL; // |=
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				return CSharpToken::EQUAL; // ==
			case '>':
				return CSharpToken::LAMBDA; // =>
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case '?':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '.':
				return CSharpToken::NULL_CONDITIONAL; // ?.
			case '?':
				return CSharpToken::NULL_COALESCE; // ??
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		/* Simple single/double character tokens (e.g. !, !=) */
		case '!':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CSharpToken::NOT_EQUAL; // !=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '%':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CSharpToken::MOD_EQUAL; // %=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '*':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CSharpToken::TIMES_EQUAL; // *=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '^':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CSharpToken::XOR_EQUAL; // ^=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		/* Operators starting with < or > */
		case '>':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* >= */
				return CSharpToken::GREATER_EQUAL; // >=
			case '>':
				src.get(c1);
				if (c1 == '=')			/* >>= */
					return CSharpToken::RSHIFT_EQUAL; // >>=
				else {			/* << */
					src.push(c1);
					return CSharpToken::RSHIFT; // >>
				}
				break;
			default:				/* > */
				src.push(c1);
				return (int)c0;
			}
			break;
		case '<':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* <= */
				return CSharpToken::LESS_EQUAL; // <=
			case '<':
				src.get(c1);
				if (c1 == '=')			/* <<= */
					return CSharpToken::LSHIFT_EQUAL; // <<=
				else {			/* << */
					src.push(c1);
					return CSharpToken::LSHIFT; // <<
				}
				break;
			default:				/* < */
				src.push(c1);
				return (int)c0;
			}
			break;
		/* Comments and / operators */
		case '/':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* /= */
				return CSharpToken::DIV_EQUAL; // /=
			case '*':				/* Block comment */
				if (process_block_comment())
					return CSharpToken::BLOCK_COMMENT; // /* ... */

				else
					return 0;
				break;
			case '/':				/* Line comment */
				c2 = src.char_after();
				if (process_line_comment()) {
					if (c2 == '/')
						return CSharpToken::DOC_COMMENT; // ///
					else
						return CSharpToken::LINE_COMMENT; // // ...
				} else
					return 0;
				break;
			default:				/* / */
				src.push(c1);
				return (int)c0;
			}
			break;
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
			bol.saw_non_space();
			val = c0;
			for (;;) {
				src.get(c0);
				if (!isalnum(c0) && c0 != '_')
					break;
				val += c0;
			}
			src.push(c0);
			key = csharp_keyword.identifier_type(val);
			switch (key) {
			case CSharpKeyword::IDENTIFIER:
				return symbols.value(val);
			case CSharpKeyword::CLASS:
			case CSharpKeyword::INTERFACE:
			case CSharpKeyword::ENUM:
			case CSharpKeyword::STRUCT:
				nesting.saw_class();
				return key;
			default:
				return key;
			}
			break;
		case '\'':
			bol.saw_non_space();
			if (process_char_literal())
				return CSharpToken::CHAR_LITERAL; // '.'
			else
				return 0;
		case '"':
			if (process_string_literal())
				return CSharpToken::STRING_LITERAL; // \"...\"
			else
				return 0;
		/* Various numbers */
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			bol.saw_non_space();
			val = c0;
			return process_number(val);
		default:
			bol.saw_non_space();
			return (int)(c0);
		}
	}
}

inline int
CSharpTokenizer::get_token()
{
	int token;

	do {
		token = get_token_real();
	} while (previous_token == CSharpToken::DOC_COMMENT && token == CSharpToken::DOC_COMMENT);
	previous_token = token;
	return token;
}

CSharpTokenizer::~CSharpTokenizer()
{
}
