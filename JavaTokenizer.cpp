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
#include "JavaTokenizer.h"
#include "JavaToken.h"

inline int
JavaTokenizer::get_token()
{
	char c0, c1, c2;
	std::string val;
	JavaKeyword::IdentifierType key;

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
			//scoped_identifier.push_back(IdMap());
			return (int)c0;
		case '}':
			bol.saw_non_space();
			//scoped_identifier.pop_back(IdMap());
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
				return JavaToken::PLUS_PLUS;
			case '=':
				return JavaToken::PLUS_EQUAL;
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
				return JavaToken::MINUS_MINUS;
			case '=':
				return JavaToken::MINUS_EQUAL;
			case '>':
				return JavaToken::ARROW;
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
				return JavaToken::BOOLEAN_AND;
			case '=':
				return JavaToken::AND_EQUAL;
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
				return JavaToken::BOOLEAN_OR;
			case '=':
				return JavaToken::OR_EQUAL;
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case ':':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case ':':		// C++ ::
				return JavaToken::DOUBLE_COLON;
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
				return JavaToken::NOT_EQUAL;
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '%':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return JavaToken::MOD_EQUAL;
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '*':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return JavaToken::TIMES_EQUAL;
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return JavaToken::EQUAL;
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '^':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return JavaToken::XOR_EQUAL;
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
				return JavaToken::GREATER_EQUAL;
			case '>':
				src.get(c1);
				if (c1 == '=')			/* >>= */
					return JavaToken::RSHIFT_EQUAL;
				else {				/* >> */
					src.push(c1);
					return JavaToken::RSHIFT;
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
				return JavaToken::LESS_EQUAL;
			case '<':
				src.get(c1);
				if (c1 == '=')			/* <<= */
					return JavaToken::LSHIFT_EQUAL;
				else {			/* << */
					src.push(c1);
					return JavaToken::LSHIFT;
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
				return JavaToken::DIV_EQUAL;
			case '*':				/* Block comment */
				if (!process_block_comment())
					return 0;
				break;
			case '/':				/* Line comment */
				if (!process_line_comment())
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
			return JavaToken::ELIPSIS;
			// Elipsis
		/* Could be a long character or string */
		case 'L':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '\'':
				if (process_char_literal())
					return JavaToken::CHAR_LITERAL;
				else
					return 0;
			case '"':
				if (process_string_literal())
					return JavaToken::STRING_LITERAL;
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
			key = java_keyword.identifier_type(val);
			if (key != JavaKeyword::IDENTIFIER)
				return key;
			else if (isupper(val[0]))
				return JavaToken::CLASS_NAME;
			else
				return JavaToken::IDENTIFIER;
				/*
				 * TODO:
				 * Classify identifiers more thoroughly
				 */
			break;
		case '\'':
			bol.saw_non_space();
			if (process_char_literal())
				return JavaToken::CHAR_LITERAL;
			else
				return 0;
		case '"':
			if (process_string_literal())
				return JavaToken::STRING_LITERAL;
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
