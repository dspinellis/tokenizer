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
#include "PythonTokenizer.h"
#include "Token.h"

/*
 * Return true if val represent a valid string or byte literal prefix
 * i.e. one of the following.
 * "r" | "u" | "R" | "U" | "f" | "F"
 * | "fr" | "Fr" | "fR" | "FR" | "rf" | "rF" | "Rf" | "RF"
 * "b" | "B" | "br" | "Br" | "bR" | "BR" | "rb" | "rB" | "Rb" | "RB"
 */
inline bool
PythonTokenizer::val_is_string_prefix()
{
	int len = val.length();

	if (len > 2)
		return false;
	switch (val[0]) {
	case 'B':
	case 'F':
		if (len == 1)
			return true;
		switch (val[1]) {
			case 'R': // BR, FR
			case 'r': // Br, Fr
				return true;
		}
		return false;
	case 'R':
	case 'r':
		if (len == 1)
			return true;
		switch (val[1]) {
			case 'B': // RB, rB
			case 'F': // RF, rF
			case 'b': // Rb, rb
			case 'f': // Rf, rf
				return true;
		}
		return false;
	case 'U':
	case 'u':
		if (len == 1)
			return true;
		else
			return false;
	case 'b':
	case 'f':
		if (len == 1)
			return true;
		switch (val[1]) {
			case 'R': // bR, fR
			case 'r': // br, fr
				return true;
		}
		return false;
	}
	return false;
}

// Process a Python string literal starting with c, returning false on EOF
bool
PythonTokenizer::process_string_literal(char c)
{
	char c0, c1, c2;
	bool triple_quoted = false;

	c1 = c2 = 0;
	bol.saw_non_space();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a string literal");
			return false;
		}
		if (c0 == '\\') {
			// Consume one character after the backslash
			src.get(c0);
			continue;
		} else if (c0 == c) {
			// Termination or triple-quoting
			if (triple_quoted) {
				if (c1 == c0 && c2 == c0)
					break;
			} else {
				if (c1 == 0) {
					// At beginning of string
					src.get(c1);
					if (c1 == c) {
						triple_quoted = true;
						c1 = 0;
						continue;
					}
					// Was empty string
					src.push(c1);
				}
				break;
			}
		}
		c2 = c1;
		c1 = c0;
	}
	return true;
}

/*
 * Main tokenizer loop, based on the rules specified in
 * https://docs.python.org/3/reference/lexical_analysis.html
 */
inline int
PythonTokenizer::get_token()
{
	char c0, c1, c2;
	Keyword::IdentifierType key;

	for (;;) {
		if (!src.get(c0))
			return 0;

		switch (c0) {
		case '\n':
		case ' ': case '\t': case '\v': case '\f': case '\r':
			break;
		case '#':				/* Line comment */
			if (process_line_comment())
				return Token::LINE_COMMENT; // #...
			else
				return 0;
			break;
		/*
		 * Double character tokens with more than 2 different outcomes
		 */
		case '-':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::MINUS_EQUAL; // -=
			case '>':
				return Token::ARROW; // ->
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
		case '&':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::AND_EQUAL; // &=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		case '@':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::AT_EQUAL; // @=
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
		case '|':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::OR_EQUAL; // |=
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
		case '+':
			bol.saw_non_space();
			src.get(c1);
			if (c1 == '=')
				return Token::PLUS_EQUAL; // +=
			else {
				src.push(c1);
				return (int)c0;
			}
			break;
		/* Operators starting with * or / */
		case '*':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::TIMES_EQUAL; 		// *=
			case '*':
				src.get(c1);
				if (c1 == '=')
					return Token::RAISE_EQUAL;	// **=
				else {
					src.push(c1);
					return Token::RAISE;		// **
				}
			default:					// *
				src.push(c1);
				return (int)c0;
			}
			break;
		case '/':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				return Token::DIV_EQUAL; 		// /=
			case '/':
				src.get(c1);
				if (c1 == '=')
					return Token::DIV_FLOOR_EQUAL;	// //=
				else {
					src.push(c1);
					return Token::DIV_FLOOR;	// //
				}
			default:					// /
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
			return Token::ELIPSIS; // ...
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

			// Handle prefixed string literals
			if ((c0 == '\'' || c0 == '"') &&
					val_is_string_prefix()) {
				if (process_string_literal(c0))
					return Token::STRING_LITERAL; // \"...\"
				else
					return 0;
			}

			src.push(c0);
			key = python_keyword.identifier_type(val);
			switch (key) {
			case Keyword::IDENTIFIER:
				return symbols.value(val);
			case Keyword::K_class:
				nesting.saw_class();
				return key;
			default:
				return key;
			}
			break;
		case '\'':
		case '"':
			if (process_string_literal(c0))
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

PythonTokenizer::~PythonTokenizer()
{
}
