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

#include <algorithm>
#include <cctype>
#include <string>
#include <cassert>
#include <cstdlib>
#include <cmath>

#include "CharSource.h"
#include "PHPTokenizer.h"
#include "Token.h"

/*
 * Process a HERE document after the <<< sequence
 * This consists of:
 * name (optionally in double quotes)
 * arbitrary text
 * name at the beginning of a line immediately followed by \n or ;
 */
bool
PHPTokenizer::process_here_document()
{
	std::string name, candidate;
	char c;

	// Skip any leading space
	for (;;) {
		if (!src.get(c))
			return false;
		if (!isspace(c))
			break;
	}

	// Read optional quote
	if (c == '"' || c == '\'')
		if (!src.get(c))
			return false;

	// Read terminating identifier
	for (;;) {
		name += c;
		if (!src.get(c))
			return false;
		if (!isalnum(c) && c != '_')
			break;
	}

	// Look for matching identifier
	bool scan_candidate = false;
	for (;;) {
		if (c == '\n') {
			candidate.clear();
			scan_candidate = true;
		} else if (scan_candidate) {
			if (isalnum(c) || c == '_')
				candidate += c;
			else {
				if ((c == '\n' || c == ';') &&
				    candidate == name)
					return true;
				scan_candidate = false;
			}
		}
		if (!src.get(c))
			return false;
	}
}

inline int
PHPTokenizer::get_token()
{
	char c0, c1, c2;
	Keyword::IdentifierType key;
	std::string lcase;

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
		case '#':				/* # line comment */
			if (process_line_comment())
				return Token::LINE_COMMENT; // #...
			else
				return 0;
			break;
			return (int)c0;
		/*
		 * Double character PHP tokens with more than 2 different outcomes
		 * (e.g. &, &=, &&)
		 */
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
				return Token::ARROW; // ->
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		/* Simple single/double character tokens (e.g. ?, ??) */
		case '?':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '?')
				return Token::NULL_COALESCE; // ??
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
		/* Operators starting with < or > or = or ! or * */
		case '>':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::GREATER_EQUAL; // >=
			case '>':
				src.get(c1);
				if (c1 == '=')
					return Token::RSHIFT_ARITHMETIC_EQUAL; // >>=
				else {
					src.push(c1);
					return Token::RSHIFT_ARITHMETIC; // >>
				}
				break;
			default:				/* > */
				src.push(c1);
				return (int)c0;
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=') {
				src.get(c1);
				if (c1 == '=')
					return Token::IDENTICAL; // ===
				else {
					src.push(c1);
					return Token::EQUAL; // ==
				}
			} else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '!':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=') {
				src.get(c1);
				if (c1 == '=')
					return Token::NOT_IDENTICAL; // !==
				else {
					src.push(c1);
					return Token::NOT_EQUAL; // !=
				}
			} else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '*':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '*':
				src.get(c1);
				if (c1 == '=')
					return Token::RAISE_EQUAL; // **=
				else {
					src.push(c1);
					return Token::RAISE; // **
				}
			case '=':
				return Token::TIMES_EQUAL; // *=
			default:
				src.push(c1);
				return (int)c0;
			}
			break;
		case '<':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				src.get(c1);
				if (c1 == '>')
					return Token::SPACESHIP; // <=>
				else {
					src.push(c1);
					return Token::LESS_EQUAL; // <=
				}
				break;
			case '<':
				src.get(c1);
				switch (c1) {
				case '=':
					return Token::LSHIFT_EQUAL; // <<=
				case '<':
					if (process_here_document())
						return Token::HERE_DOCUMENT; // <<<...
					else {
						error("EOF encountered while processing a here document");
						return 0;
					}
				default:
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
				if (process_block_comment())
					return Token::BLOCK_COMMENT; // /*...*/
				else
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
		case '.':	/* . and ... */
			bol.saw_non_space();
			src.get(c1);
			if (isdigit(c1)) {
				val = std::string(".") + (char)(c1);
				return process_number(val);
			}
			switch (c1) {
			case '.':
				src.get(c2);
				if (c2 != '.') {
					src.push(c2);
					src.push(c1);
					return (int)c0;
				}
				return Token::ELIPSIS; // ...
			case '=':
				return Token::CONCAT_EQUALS; // .=
			default:
				src.push(c1);
				return (int)c0;
			}
		/* XXX Can also be non-ASCII */
		case '$': case '\\':
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
				// Namespace prefix treated as single name
				if (!isalnum(c0) && c0 != '_' && c0 != '\\')
					break;
				val += c0;
			}
			src.push(c0);

			lcase = val;
			std::transform(lcase.begin(), lcase.end(), lcase.begin(), ::tolower);
			key = php_keyword.identifier_type(lcase);
			if (key == Keyword::IDENTIFIER)
				return symbols.value(val);
			else
				return key;
			break;
		case '\'':
			bol.saw_non_space();
			if (process_char_literal())
				return Token::CHAR_LITERAL; // '.'
			else
				return 0;
		case '"':
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

PHPTokenizer::~PHPTokenizer()
{
}
