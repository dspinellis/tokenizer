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
#include "GoTokenizer.h"
#include "Token.h"

inline token_type
GoTokenizer::get_immediate_token()
{
	char c0, c1, c2;
	Keyword::IdentifierType key;

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
			break;
		case ' ': case '\t': case '\v': case '\f': case '\r':
			break;
		case '{':
			bol.saw_non_space();
			symbols.enter_scope();
			nesting.saw_open_brace();
			return static_cast<token_type>(c0);
		case '}':
			bol.saw_non_space();
			symbols.exit_scope();
			nesting.saw_close_brace();
			return static_cast<token_type>(c0);
		case ';':
			bol.saw_non_space();
			nesting.unsaw_class();
			return static_cast<token_type>(c0);
		/*
		 * Double character C tokens with more than 2 different outcomes
		 * (e.g. +, +=, ++)
		 */
		case '+':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '+':
				return Token::PLUS_PLUS; // ++
			case '=':
				return Token::PLUS_EQUAL; // +=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '-':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '-':
				return Token::MINUS_MINUS; // --
			case '=':
				return Token::MINUS_EQUAL; // -=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '|':
			bol.saw_non_space();
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
			break;
		/* Simple single/double character tokens (e.g. !, !=) */
		case '!':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::NOT_EQUAL; // !=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '%':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::MOD_EQUAL; // %=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '*':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::TIMES_EQUAL; // *=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::EQUAL; // ==
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '^':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::XOR_EQUAL; // ^=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case ':':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::VAR_ASSIGN; // :=
			else {
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		/* Operators with three characters */
		case '&':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '&':
				return Token::BOOLEAN_AND; // &&
			case '^': // &^
				src.get(c1);
				if (c1 == '=')
					return Token::AND_NOT_EQUAL; // &^=
				else {
					src.push(c1);
					return Token::AND_NOT; // &^
				}
				break;
			case '=':
				return Token::AND_EQUAL; // &=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '>':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* >= */
				return Token::GREATER_EQUAL; // >=
			case '>':
				src.get(c1);
				if (c1 == '=')			/* >>= */
					return Token::RSHIFT_EQUAL; // >>=
				else {				/* >> */
					src.push(c1);
					return Token::RSHIFT; // >>
				}
				break;
			default:				/* > */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '<':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::LESS_EQUAL; // <=
			case '-':
				return Token::CHANNEL_DIRECTION; // <-
			case '<':
				src.get(c1);
				if (c1 == '=')
					return Token::LSHIFT_EQUAL; // <<=
				else {
					src.push(c1);
					return Token::LSHIFT; // <<
				}
				break;
			default:				/* < */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		/* Comments and / operators */
		case '/':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* /= */
				return Token::DIV_EQUAL; // /=
			case '*':				/* Block comment */
				return process_block_comment();
			case '/':				/* Line comment */
				return process_line_comment();
			default:				/* / */
				src.push(c1);
				return static_cast<token_type>(c0);
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
				return static_cast<token_type>(c0);
			}
			src.get(c2);
			if (c2 != '.') {
				src.push(c2);
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			return Token::ELIPSIS; // ...
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
			key = go_keyword.identifier_type(val);
			switch (key) {
			case Keyword::FIRST_IDENTIFIER:
				return symbols.value(val);
			case Keyword::FIRST:
			case Keyword::LAST:
				assert(false);
			case Keyword::K_struct:
				nesting.saw_class();
			default:
				return key;
			}
			break;
		case '\'':
			bol.saw_non_space();
			if (process_char_literal())
				return Token::CHAR_LITERAL; // '.'
			else
				return 0;
		case '"':
			bol.saw_non_space();
			if (process_string_literal())
				return Token::STRING_LITERAL; // \"...\"
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
			return static_cast<token_type>(c0);
		}
	}
}

GoTokenizer::~GoTokenizer()
{
}
