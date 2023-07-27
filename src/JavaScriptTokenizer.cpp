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
#include "JavaScriptTokenizer.h"
#include "Token.h"

// Process a JavaScript string literal starting with c, returning false on EOF
bool
JavaScriptTokenizer::process_string_literal(char c)
{
	char c0;
	bool saw_dollar = false;
	bool is_template = (c == '`');

	bol.saw_non_space();
	if (all_contents)
		sequence_hash.reset();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a string literal");
			return false;
		}
		if (c0 == '\\') {
			// Consume one character after the backslash
			src.get(c0);
			if (all_contents)
				sequence_hash.add(c0);
		} else if (is_template && c0 == '$') {
			saw_dollar = true;
		} else if (saw_dollar && c0 == '{') {
			// Process JS code in template string
			int brace_depth = 1;
			do {
				token_type c = get_token();
				if (all_contents)
					sequence_hash.add(c);
				if (c == '{')
					++brace_depth;
				else if (c ==  '}')
					--brace_depth;
				else if (c == 0) {
					error("EOF encountered while processing a code in template string");
					return false;
				}
			} while (brace_depth);
		} else if (c0 == c) {
			// Termination
			break;
		} else {
			saw_dollar = false;
		}

		if (all_contents)
			sequence_hash.add(c0);
	}
	if (all_contents)
		push_token(sequence_hash.get());
	return true;
}

// Process a JavaScript regular expression literal, returning false on EOF
bool
JavaScriptTokenizer::process_regex_literal()
{
	char c0;

	bol.saw_non_space();
	if (all_contents)
		sequence_hash.reset();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a regex literal");
			return false;
		}
		if (c0 == '\\') {
			// Consume one character after the backslash
			src.get(c0);
			if (all_contents)
				sequence_hash.add(c0);
		} else if (c0 == '/') {
			// Termination; read flags
			for (;;) {
				src.get(c0);
				if (!isalnum(c0))
					break;
				if (all_contents)
					sequence_hash.add(c0);
			}
			src.push(c0);
			break;
		}
		if (all_contents)
			sequence_hash.add(c0);
	}
	if (all_contents)
		push_token(sequence_hash.get());
	return true;
}


/*
 * Return a token, maintaining the current context
 * (within an expression or not)
 */
inline token_type
JavaScriptTokenizer::get_immediate_token()
{
	token_type t = get_token_for_context();

	// Some elements don't modify the state
	if (TokenId::is_horizontal_space(t)
	    || TokenId::is_hashed_content(t)
	    || t == static_cast<token_type>(Token::BLOCK_COMMENT)
	    || t == static_cast<token_type>(Token::LINE_COMMENT)
	   )
		return t;

	// A / or /= can appear after the following cases
	expression_context = (TokenId::is_identifier(t)
	    || TokenId::is_number(t)
	    || t == static_cast<token_type>(')')
	    || t == static_cast<token_type>(']')
	    || t == static_cast<token_type>(Token::PLUS_PLUS)
	    || t == static_cast<token_type>(Token::MINUS_MINUS)
	   );
	return t;

}

/*
 * Return a JavaScript token for the current context.
 * In an expression context "/" is a division or /= operator.
 * Otherwise, it begins a regex literal scan.
 */
inline token_type
JavaScriptTokenizer::get_token_for_context()
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
			return static_cast<token_type>(c0);
		case '}':
			bol.saw_non_space();
			symbols.exit_scope();
			nesting.saw_close_brace();
			return static_cast<token_type>(c0);
		case ';':
			bol.saw_non_space();
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
		/* Simple single/double character tokens (e.g. !, !=) */
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
		// Tripple character operators
		case '*':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':				/* *= */
				return Token::TIMES_EQUAL;
			case '*':
				src.get(c1);
				switch (c1) {
				case '=':	/* **= */
					return Token::RAISE_EQUAL; // **=
				default:
					src.push(c1);
					return Token::RAISE; // **
				}
				break;
			default:				/* * */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '=':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '>':
				return Token::LAMBDA; // =>
			case '=':
				src.get(c1);
				switch (c1) {
				case '=':	/* === */
					return Token::EQUAL_STRICT; // ===
				default:
					src.push(c1);
					return Token::EQUAL; // ==
				}
				break;
			default:				/* = */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '!':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '=':
				src.get(c1);
				switch (c1) {
				case '=':	/* !== */
					return Token::NOT_EQUAL_STRICT; // !===
				default:
					src.push(c1);
					return Token::NOT_EQUAL; // !=
				}
				break;
			default:				/* ! */
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '&':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '&': /* && */
				src.get(c1);
				switch (c1) {
				case '=':
					return Token::BOOLEAN_AND_EQUAL; // &&=
				default:
					src.push(c1);
					return Token::BOOLEAN_AND; // &&
				}
				break;
			case '=':
				return Token::AND_EQUAL; // &=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '|':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '|': /* || */
				src.get(c1);
				switch (c1) {
				case '=':
					return Token::BOOLEAN_OR_EQUAL; // ||=
				default:
					src.push(c1);
					return Token::BOOLEAN_OR; // ||
				}
				break;
			case '=':
				return Token::OR_EQUAL; // |=
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
			}
			break;
		case '?':
			bol.saw_non_space();
			src.get(c1);
			switch (c1) {
			case '?':
				src.get(c1);
				switch (c1) {
				case '=':
					// Avoid trigraph in string representation
					return Token::NULL_COALESCING_EQUAL; // ??" "=
				default:
					src.push(c1);
					return Token::NULL_COALESCING; // ??
				}
				break;
			case '.':
				return Token::NULL_CONDITIONAL; // ?.
			default:
				src.push(c1);
				return static_cast<token_type>(c0);
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
				switch (c1) {
				case '=':	/* >>= */
					return Token::RSHIFT_ARITHMETIC_EQUAL; // >>=
				case '>':	/* >>> */
					src.get(c2);
					if (c2 == '=')
						return Token::RSHIFT_LOGICAL_EQUAL; // >>>=
					else {
						src.push(c2);
						return Token::RSHIFT_LOGICAL; // >>>
					}
				default:
					src.push(c1);
					return Token::RSHIFT_ARITHMETIC; // >>
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
				return static_cast<token_type>(c0);
			}
			break;
		/* Comments and / operators */
		case '/':
			bol.saw_non_space();
			src.get(c1);
			if (expression_context)
				switch (c1) {
				case '=':				/* /= */
					return Token::DIV_EQUAL; // /=
				case '*':		/* Block comment */
					return get_block_comment_token();
				case '/':		/* Line comment */
					return get_line_comment_token();
				default:		/* / */
					src.push(c1);
					return static_cast<token_type>(c0);
				}
			else // Non-expression context
				switch (c1) {
				case '*':		/* Block comment */
					return get_block_comment_token();
				case '/':		/* Line comment */
					return get_line_comment_token();
				default:		/* / */
					src.push(c1);
					if (process_regex_literal())
						return Token::REGEX_LITERAL; // /.../
					else
						return 0;
				}
		case '.':	/* . and ... */
			bol.saw_non_space();
			src.get(c1);
			if (isdigit(c1)) {
				val = std::string(".") + (char)(c1);
				return get_number_token(val);
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
		/* XXX Can also be non-ASCII */
		case '_': case 'a': case 'b': case 'c': case 'd': case 'e':
		case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
		case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
		case 'r': case 's': case 't': case 'u': case 'v': case 'w':
		case 'x': case 'y': case 'z': case 'A': case 'B': case 'C':
		case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O':
		case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'V': case 'W': case 'X': case 'Y': case 'Z': case '$':
			bol.saw_non_space();
			val = c0;
			for (;;) {
				src.get(c0);
				if (!isalnum(c0) && c0 != '_' && c0 != '$')
					break;
				val += c0;
			}
			src.push(c0);
			key = keyword.identifier_type(val);
			switch (key) {
			case Keyword::FIRST_IDENTIFIER:
				return symbols.value(val);
			case Keyword::K_class:
			case Keyword::K_interface:
			case Keyword::K_enum:
				nesting.saw_class();
				return key;
			default:
				return key;
			}
			break;
		case '`':
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
			return get_number_token(val);
		default:
			bol.saw_non_space();
			return static_cast<token_type>(c0);
		}
	}
}

JavaScriptTokenizer::~JavaScriptTokenizer()
{
}
