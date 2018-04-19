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

static inline bool
is_eol_char(char c) {
	return (c == '\r' || c == '\n');
}

/*
 * Convert the passed number into an integer token value, in the range
 * 1000-2000, with 0 being 1500 and the rest being represented through
 * base 10 logarithms.
 */
static int
num_token(const std::string &val)
{
	const int BASE = 1500;

	double d = strtod(val.c_str(), NULL);
	if (d == 0)
		return BASE;
	d = log(d) / log(10);
	if (d >= 0)
		d = ceil(d) + 1;
	if (d < 0)
		d = floor(d);
	return BASE + d;
}

inline bool
CTokenizer::process_character()
{
	char c0, c1, c2;
	std::string val;
	CKeyword::IdentifierType key;
	BolState bol;

#define GET(x) do { \
	if (!src.get(x)) \
		return false; \
} while (0)

	GET(c0);
	switch (c0) {
	/*
	 * Single character C operators and punctuators
	 * ANSI 3.1.5 p. 32 and 3.1.6 p. 33
	 */
	case '\n':
		scan_cpp_line = false;
		break;
	case ' ': case '\t': case '\v': case '\f': case '\r':
		break;
	case '?':
	case '[':
	case '(':
	case '~':
	case ',':
	case ']':
	case ')':
		bol.saw_non_space();
		std::cout << (int)c0 << ' ';
		break;
	case '{':
		bol.saw_non_space();
		//scoped_identifier.push_back(IdMap());
		std::cout << (int)c0 << ' ';
		break;
	case '}':
		bol.saw_non_space();
		//scoped_identifier.pop_back(IdMap());
		std::cout << (int)c0 << ' ';
		break;
	case ';':
		bol.saw_non_space();
		std::cout << (int)c0 << ' ';
		break;
	/*
	 * Double character C tokens with more than 2 different outcomes
	 * (e.g. +, +=, ++)
	 */
	case '+':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '+':
			std::cout << CToken::PLUS_PLUS << ' ';
			break;
		case '=':
			std::cout << CToken::PLUS_EQUAL << ' ';
			break;
		default:
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		break;
	case '-':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '-':
			std::cout << CToken::MINUS_MINUS << ' ';
			break;
		case '=':
			std::cout << CToken::MINUS_EQUAL << ' ';
			break;
		case '>':
			std::cout << CToken::ARROW << ' ';
			break;
		default:
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		break;
	case '&':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '&':
			std::cout << CToken::BOOLEAN_AND << ' ';
			break;
		case '=':
			std::cout << CToken::AND_EQUAL << ' ';
			break;
		default:
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		break;
	case '|':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '|':
			std::cout << CToken::BOOLEAN_OR << ' ';
			break;
		case '=':
			std::cout << CToken::OR_EQUAL << ' ';
			break;
		default:
			src.push(c1);
			std::cout << (int)c0 << ' ';
		}
		break;
	case ':':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case ':':		// C++ ::
			std::cout << CToken::DOUBLE_COLON << ' ';
			break;
		default:
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		break;
	/* Simple single/double character tokens (e.g. !, !=) */
	case '!':
		bol.saw_non_space();
		GET(c1);
		if (c1 == '=')
			std::cout << CToken::NOT_EQUAL << ' ';
		else {
			src.push(c1);
			std::cout << (int)c0 << ' ';
		}
		break;
	case '%':
		bol.saw_non_space();
		GET(c1);
		if (c1 == '=')
			std::cout << CToken::MOD_EQUAL << ' ';
		else {
			src.push(c1);
			std::cout << (int)c0 << ' ';
		}
		break;
	case '*':
		bol.saw_non_space();
		GET(c1);
		if (c1 == '=')
			std::cout << CToken::TIMES_EQUAL << ' ';
		else {
			src.push(c1);
			std::cout << (int)c0 << ' ';
		}
		break;
	case '=':
		bol.saw_non_space();
		GET(c1);
		if (c1 == '=')
			std::cout << CToken::EQUAL << ' ';
		else {
			src.push(c1);
			std::cout << (int)c0 << ' ';
		}
		break;
	case '^':
		bol.saw_non_space();
		GET(c1);
		if (c1 == '=')
			std::cout << CToken::XOR_EQUAL << ' ';
		else {
			src.push(c1);
			std::cout << (int)c0 << ' ';
		}
		break;
	case '#':
		bol.saw_non_space();
		if (bol.at_bol_space()) {
			scan_cpp_directive = true;
			scan_cpp_line = true;
		}
		bol.saw_non_space();
		std::cout << (int)c0 << ' ';
		break;
	/* Operators starting with < or > */
	case '>':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '=':				/* >= */
			std::cout << CToken::GREATER_EQUAL << ' ';
			break;
		case '>':
			GET(c1);
			if (c1 == '=')			/* >>= */
				std::cout << CToken::RSHIFT_EQUAL << ' ';
			else {				/* >> */
				src.push(c1);
				std::cout << CToken::RSHIFT << ' ';
			}
			break;
		default:				/* > */
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		break;
	case '<':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '=':				/* <= */
			std::cout << CToken::LESS_EQUAL << ' ';
			break;
		case '<':
			GET(c1);
			if (c1 == '=')			/* <<= */
				std::cout << CToken::LSHIFT_EQUAL << ' ';
			else {			/* << */
				src.push(c1);
				std::cout << CToken::LSHIFT << ' ';
			}
			break;
		default:				/* < */
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		break;
	/* Comments and / operators */
	case '/':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '=':				/* /= */
			std::cout << CToken::DIV_EQUAL << ' ';
			break;
		case '*':				/* Block comment */
			GET(c1);
			for (;;) {
				while (c1 != '*') {
					if (!isspace(c0) && bol.at_bol_space())
						bol.saw_non_space();
					GET(c1);
					if (c1 == '\n')
						newline(true);
				}
				if (!isspace(c1) && bol.at_bol_space())
					bol.saw_non_space();
				GET(c1);
				if (c1 == '/')
					break;
				else if (c1 == '\n')
						newline();
			}
			break;
		case '/':				/* Line comment */
			GET(c1);
			for (;;) {
				if (c1 == '\n')
					break;
				GET(c1);
			}
			src.push(c1);
			break;
		default:				/* / */
			src.push(c1);
			break;
		}
		break;
	case '.':	/* . and ... */
		bol.saw_non_space();
		GET(c1);
		if (isdigit(c1)) {
			val = std::string(".") + (char)(c1);
			goto number;
		}
		if (c1 != '.') {
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		GET(c2);
		if (c2 != '.') {
			src.push(c2);
			src.push(c1);
			std::cout << (int)c0 << ' ';
			break;
		}
		std::cout << CToken::ELIPSIS << ' ';
		// Elipsis
		break;
	/* Could be a long character or string */
	case 'L':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '\'':
			goto char_literal;
		case '"':
			goto string_literal;
		default:
			src.push(c1);
			goto identifier;
		}
	case '_': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
	case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'M': case 'N': case 'O':
	case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z':
	identifier:
		bol.saw_non_space();
		val = c0;
		for (;;) {
			GET(c0);
			if (!isalnum(c0) && c0 != '_')
				break;
			val += c0;
		}
		src.push(c0);
		key = ckeyword.identifier_type(val);
		switch (key) {
		case CKeyword::FOR:
		case CKeyword::WHILE:
		case CKeyword::CASE:
		case CKeyword::CONST:
		case CKeyword::DEFAULT:
		case CKeyword::GOTO:
		case CKeyword::REGISTER:
		case CKeyword::SIGNED:
		case CKeyword::STATIC:
		case CKeyword::TYPEDEF:
		case CKeyword::ENUM:
		case CKeyword::INLINE:
		case CKeyword::NOALIAS:
		case CKeyword::RESTRICT:
		case CKeyword::STRUCT:
		case CKeyword::UNION:
		case CKeyword::UNSIGNED:
		case CKeyword::VOID:
		case CKeyword::VOLATILE:
		case CKeyword::DO:
		case CKeyword::SWITCH:
		case CKeyword::BREAK:
		case CKeyword::CONTINUE:
		case CKeyword::RETURN:
		case CKeyword::AUTO:
		case CKeyword::CHAR:
		case CKeyword::DOUBLE:
		case CKeyword::EXTERN:
		case CKeyword::FLOAT:
		case CKeyword::INT:
		case CKeyword::LONG:
		case CKeyword::SHORT:
		case CKeyword::SIZEOF:
		case CKeyword::ELSE:
		case CKeyword::IF:
			std::cout << key << ' ';
			break;
		case CKeyword::IFDEF:
		case CKeyword::ELIF:
		case CKeyword::INCLUDE:
			if (scan_cpp_directive) {
				std::cout << key << ' ';
				break;
			}
			/* FALLTHROUGH */
		case CKeyword::IDENTIFIER:
			/*
			 * TODO:
			 * Use consistent token ids for all C library identifiers
			 * Use same token ids for global scope identifiers
			 * Use unique token ids for identifiers in each scope
			 */
			std::cout << CToken::IDENTIFIER << ' ';
			break;
		case CKeyword::LAST:
			assert(false);
		}
		scan_cpp_directive = false;
		break;
	case '\'':
		bol.saw_non_space();
	char_literal:
		val = "";
		for (;;) {
			GET(c0);
			if (c0 == '\\') {
				// Consume one character after the backslash
				// ... to deal with the '\'' problem
				val += '\\';
				GET(c0);
				val += c0;
				continue;
			}
			if (c0 == '\'')
				break;
			val += c0;
		}
		break;
	case '"':
		bol.saw_non_space();
	string_literal:
		val = "";
		for (;;) {
			GET(c0);
			if (c0 == '\\') {
				val += '\\';
				// Consume one character after the backslash
				GET(c0);
				if (c0 == '\n')
					newline(true);
				val += c0;
				continue;
			} else if (c0 == '"')
				break;
			else if (c0 == '\n')
				newline(true);
			val += c0;
		}
		std::cout << CToken::IDENTIFIER << ' ';
		break;
	/* Various numbers */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		bol.saw_non_space();
		val = c0;
	number:
		for (;;) {
			GET(c0);
			if (c0 == 'e' || c0 == 'E') {
				val += c0;
				GET(c0);
				if (c0 == '+' || c0 == '-') {
					val += c0;
					continue;
				}
			}
			if (!isalnum(c0) && c0 != '.' && c0 != '_')
				break;
			val += c0;
		}
		src.push(c0);
		std::cout << num_token(val) << ' ';
		break;
	default:
		break;
	}
	return true;
}

void
CTokenizer::tokenize()
{
	while (process_character())
		;
	std::cout << std::endl;
}
