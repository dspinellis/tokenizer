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
#include "CppTokenizer.h"
#include "Token.h"

inline int
CppTokenizer::get_token()
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
			return (int)c0;
		case '}':
			bol.saw_non_space();
			symbols.exit_scope();
			nesting.saw_close_brace();
			return (int)c0;
		case ';':
			bol.saw_non_space();
			/*
			 * class might have been used as a forwar declaration
			 * or an elaborated type.
			 */
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
				return Token::PLUS_PLUS; // ++
			case '=':
				return Token::PLUS_EQUAL; // +=
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
				return Token::MINUS_MINUS; // --
			case '=':
				return Token::MINUS_EQUAL; // -=
			case '>':
				src.get(c2);
				if (c2 == '*')
					return Token::MEMBER_PTR_FROM_OBJECT_PTR; // ->*
				else {
					src.push(c2);
					return Token::MEMBER_FROM_OBJECT_PTR; // ->
				}
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
				return Token::BOOLEAN_AND; // &&
			case '=':
				return Token::AND_EQUAL; // &=
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
				return Token::BOOLEAN_OR; // ||
			case '=':
				return Token::OR_EQUAL; // |=
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
				return Token::NOT_EQUAL; // !=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '%':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::MOD_EQUAL; // %=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '*':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::TIMES_EQUAL; // *=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::EQUAL; // ==
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case ':':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == ':')
				return Token::SCOPE; // ::
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '^':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::XOR_EQUAL; // ^=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '#':
			src.get(c1);
			if (c1 == '#')
				return Token::TOKEN_PASTE; // ##
			else
				src.push(c1);
			if (bol.at_bol_space())
				scan_cpp_directive = true;
			bol.saw_non_space();
			return (int)c0;
		/* Operators starting with < or > */
		case '>':
			bol.saw_non_space();
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
				return Token::LESS_EQUAL; // <=
			case '<':
				src.get(c1);
				if (c1 == '=')			/* <<= */
					return Token::LSHIFT_EQUAL; // <<=
				else {			/* << */
					src.push(c1);
					return Token::LSHIFT; // <<
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
				return Token::DIV_EQUAL; // /=
			case '*':				/* Block comment */
				c2 = src.char_after();
				if (process_block_comment()) {
					if (c2 == '*')
						return Token::JAVADOC_COMMENT; // /**...*/
					else
						return Token::BLOCK_COMMENT; // /*...*/

				} else
					return 0;
				break;
			case '/':				/* Line comment */
				if (process_line_comment())
					return Token::LINE_COMMENT; // //...
				else
					return 0;
				break;
			default:				/* / */
				src.push(c1);
				return (int)c0;
			}
			break;
		case '.':	/* . .* ... */
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				val = std::string(".") + (char)(c1);
				return process_number(val);
			case '.':
				src.get(c2);
				if (c2 == '.')
					return Token::ELIPSIS; // ...
				else {
					src.push(c2);
					src.push(c1);
					return (int)c0;
				}
				break;
			case '*':
				return Token::MEMBER_PTR_FROM_OBJECT; // .*
			default:
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
			key = cpp_keyword.identifier_type(val);
			switch (key) {
			case Keyword::K_define:
			case Keyword::K_elif:
			case Keyword::K_endif:
			case Keyword::K_error:
			case Keyword::K_ifdef:
			case Keyword::K_ifndef:
			case Keyword::K_include:
			case Keyword::K_line:
			case Keyword::K_pragma:
			case Keyword::K_undef:
				if (scan_cpp_directive) {
					scan_cpp_directive = false;
					return key;
				} else
					return symbols.value(val);
				break;
			case Keyword::IDENTIFIER:
				return symbols.value(val);
			case Keyword::K_class:
			case Keyword::K_struct:
				nesting.saw_class();
				return key;
			// Alternative representations of standard tokens
			case Keyword::K_and:
				return Token::BOOLEAN_AND; // &&
			case Keyword::K_bitor:
				return '|';
			case Keyword::K_or:
				return Token::BOOLEAN_OR; // ||
			case Keyword::K_xor:
				return '^';
			case Keyword::K_compl:
				return '~';
			case Keyword::K_bitand:
				return '&';
			case Keyword::K_and_eq:
				return Token::AND_EQUAL; // &=
			case Keyword::K_or_eq:
				return Token::OR_EQUAL; // |=
			case Keyword::K_xor_eq:
				return Token::XOR_EQUAL; // ^=
			case Keyword::K_not:
				return '!';
			case Keyword::K_not_eq:
				return Token::NOT_EQUAL; // !=
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
			return (int)(c0);
		}
	}
}

CppTokenizer::~CppTokenizer()
{
}
