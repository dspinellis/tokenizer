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

#include "BolState.h"
#include "CharSource.h"
#include "CTokenizer.h"
#include "CToken.h"

inline int
CTokenizer::get_token()
{
	char c0, c1, c2;
	CKeyword::IdentifierType key;

	for (;;) {
		if (!src.get(c0))
			return 0;

		switch (c0) {
		/*
		 * Single character C operators and punctuators
		 * ANSI 3.1.5 p. 32 and 3.1.6 p. 33
		 */
		case '\n':
			bol.saw_newline();
			scan_cpp_line = false;
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
			nesting.unsaw_class();
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
				return CToken::PLUS_PLUS; // ++
			case '=':
				return CToken::PLUS_EQUAL; // +=
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
				return CToken::MINUS_MINUS; // --
			case '=':
				return CToken::MINUS_EQUAL; // -=
			case '>':
				return CToken::ARROW; // ->
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
				return CToken::BOOLEAN_AND; // &&
			case '=':
				return CToken::AND_EQUAL; // &=
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
				return CToken::BOOLEAN_OR; // ||
			case '=':
				return CToken::OR_EQUAL; // |=
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
				return CToken::NOT_EQUAL; // !=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '%':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CToken::MOD_EQUAL; // %=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '*':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CToken::TIMES_EQUAL; // *=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CToken::EQUAL; // ==
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '^':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return CToken::XOR_EQUAL; // ^=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '#':
			src.get(c1);
			if (c1 == '#')
				return CToken::TOKEN_PASTE; // ##
			else
				src.push(c1);
			if (bol.at_bol_space()) {
				scan_cpp_directive = true;
				scan_cpp_line = true;
			}
			bol.saw_non_space();
			return (int)c0;
		/* Operators starting with < or > */
		case '>':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* >= */
				return CToken::GREATER_EQUAL; // >=
			case '>':
				src.get(c1);
				if (c1 == '=')			/* >>= */
					return CToken::RSHIFT_EQUAL; // >>=
				else {				/* >> */
					src.push(c1);
					return CToken::RSHIFT; // >>
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
				return CToken::LESS_EQUAL; // <=
			case '<':
				src.get(c1);
				if (c1 == '=')			/* <<= */
					return CToken::LSHIFT_EQUAL; // <<=
				else {			/* << */
					src.push(c1);
					return CToken::LSHIFT; // <<
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
				return CToken::DIV_EQUAL; // /=
			case '*':				/* Block comment */
				c2 = src.char_after();
				if (process_block_comment()) {
					if (c2 == '*' || c2 == '!')
						return CToken::DOC_COMMENT; // /** ... */
					else
						return CToken::BLOCK_COMMENT; // /* ... */

				} else
					return 0;
				break;
			case '/':				/* Line comment */
				if (process_line_comment())
					return CToken::LINE_COMMENT; // // ...
				else
					return 0;
				break;
			default:				/* / */
				src.push(c1);
				return (int)c0;
			}
			break;
		case '.':	/* . and ... */
			bol.saw_non_space();
			src.get(c1);
			if (isdigit(c1)) {
				val = std::string(".") + (char)(c1);
				return process_number(val);
			}
			if (c1 != '.') {
				src.push(c1);
				return (int)c0;
			}
			src.get(c2);
			if (c2 != '.') {
				src.push(c2);
				src.push(c1);
				return (int)c0;
			}
			return CToken::ELIPSIS; // ...
			// Elipsis
		/* Could be a long character or string */
		case 'L':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '\'':
				if (process_char_literal())
					return CToken::CHAR_LITERAL; // '.'
				else
					return 0;
			case '"':
				if (process_string_literal())
					return CToken::STRING_LITERAL; // \"...\"
				else
					return 0;
			default:
				src.push(c1);
				goto identifier;
			}
		case '_': case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
		case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
		case 'r': case 's': case 't': case 'u': case 'v': case 'w':
		case 'x': case 'y': case 'z': case 'A': case 'B': case 'C':
		case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'M': case 'N': case 'O': case 'P':
		case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
		case 'W': case 'X': case 'Y': case 'Z':
		identifier:
			bol.saw_non_space();
			val = c0;
			for (;;) {
				src.get(c0);
				if (!isalnum(c0) && c0 != '_')
					break;
				val += c0;
			}
			src.push(c0);
			key = ckeyword.identifier_type(val);
			switch (key) {
			case CKeyword::IFDEF:
			case CKeyword::ELIF:
			case CKeyword::INCLUDE:
				if (scan_cpp_directive)
					return key;
				else
					return symbols.value(val);
				break;
			case CKeyword::IDENTIFIER:
				return symbols.value(val);
			case CKeyword::FIRST:
			case CKeyword::LAST:
				assert(false);
			case CKeyword::STRUCT:
				nesting.saw_class();
			default:
				return key;
			}
			scan_cpp_directive = false;
			break;
		case '\'':
			bol.saw_non_space();
			if (process_char_literal())
				return CToken::CHAR_LITERAL; // '.'
			else
				return 0;
		case '"':
			bol.saw_non_space();
			if (process_string_literal())
				return CToken::STRING_LITERAL; // \"...\"
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

CTokenizer::~CTokenizer()
{
}
