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
#include <cassert>
#include <string>
#include <cassert>
#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <sstream>

#include "BolState.h"
#include "CharSource.h"
#include "TokenizerBase.h"
#include "Token.h"
#include "TokenId.h"

const double LOG10 = std::log(10.0);

/*
 * Compress the passed number into its base 10 logarithm
 * and return its token number.
 */
token_type
TokenizerBase::compress(double d)
{
	d = std::log(d) / LOG10;
	if (d >= 0)
		d = ceil(d) + 1;
	if (d < 0)
		d = floor(d);
	return TokenId::NUMBER_ZERO + static_cast<token_type>(d);
}
/*
 * Convert the passed number into an integer token value, in the range
 * NUMBER_ZERO +- 500, with 0 being NUMBER_ZERO and the rest being
 * represented through base 10 logarithms.
 */
token_type
TokenizerBase::num_token(const std::string &val)
{

	double d = strtod(val.c_str(), NULL);

	switch (std::fpclassify(d)) {
	case FP_INFINITE: return TokenId::NUMBER_INFINITE;
	case FP_NAN: return TokenId::NUMBER_NAN;
	case FP_ZERO: return TokenId::NUMBER_ZERO;
	default:
	      break;
	}
	return compress(d);
}

// Process a block comment, returning the token's code
token_type
TokenizerBase::get_block_comment_token()
{
	char c1;
	token_type ret = Token::BLOCK_COMMENT; // /*...*/

	src.get(c1);

	if (all_contents) {
		sequence_hash.reset();
		sequence_hash.add(c1);
	}

	// Comment starts with /**, and isn't empty (/**/)
	if (c1 == '*' && src.char_after() != '/')
		ret = Token::BLOCK_DOC_COMMENT; // /**...*/

	for (;;) {
		while (c1 != '*') {
			if (!isspace(c1) && bol.at_bol_space())
				bol.saw_non_space();
			if (!src.get(c1)) {
				error("EOF encountered while processing a block comment");
				return 0;
			}
			if (all_contents)
				sequence_hash.add(c1);
		}
		if (!isspace(c1) && bol.at_bol_space())
			bol.saw_non_space();
		if (!src.get(c1)) {
			error("EOF encountered while processing a block comment");
			return 0;
		}
		if (c1 == '/')
			break;
		if (all_contents)
			sequence_hash.add(c1);
	}
	if (all_contents)
		push_token(sequence_hash.get());
	return ret;
}

// Process a line comment, returning the token type
token_type
TokenizerBase::get_line_comment_token()
{
	char c1;
	token_type ret = Token::LINE_COMMENT; // //...

	src.get(c1);

	if (all_contents) {
		sequence_hash.reset();
		sequence_hash.add(c1);
	}

	if (c1 == '/')
		ret = Token::LINE_DOC_COMMENT; // ///...

	for (;;) {
		if (c1 == '\n')
			break;
		if (!src.get(c1)) {
			if (all_contents)
				push_token(sequence_hash.get());
			return ret;
		}
		if (all_contents)
			sequence_hash.add(c1);
	}
	src.push(c1);

	if (all_contents)
		push_token(sequence_hash.get());

	return ret;
}

// Process a character literal, returning false on EOF
bool
TokenizerBase::process_char_literal()
{
	char c0;	// Each character read

	if (all_contents)
		sequence_hash.reset();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a character literal");
			return false;
		}
		if (c0 == '\\') {
			// Consume one character after the backslash
			// ... to deal with the '\'' problem
			src.get(c0);
			if (all_contents)
				sequence_hash.add(c0);
			continue;
		}
		if (c0 == '\'')
			break;
		if (all_contents)
			sequence_hash.add(c0);
	}
	if (all_contents)
		push_token(sequence_hash.get());
	return true;
}

// Process a string literal, returning false on EOF
bool
TokenizerBase::process_string_literal()
{
	char c0;

	bol.saw_non_space();
	if (all_contents)
		sequence_hash.reset();
	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a string literal");
			return false;
		}
		if (all_contents)
			sequence_hash.add(c0);
		if (c0 == '\\') {
			// Consume one character after the backslash
			src.get(c0);
			if (all_contents)
				sequence_hash.add(c0);
			continue;
		} else if (c0 == '"')
			break;
	}
	if (all_contents)
		push_token(sequence_hash.get());
	return true;
}

// Process a number starting with the passed string returning its token value
token_type
TokenizerBase::get_number_token(std::string &val)
{
	char c0;

	for (;;) {
		src.get(c0);
		if (c0 == 'e' || c0 == 'E') {
			val += c0;
			src.get(c0);
			if (c0 == '+' || c0 == '-') {
				val += c0;
				continue;
			}
		}
		// Decimal and digit separators
		if (!isalnum(c0) && c0 != '.' && c0 != '_' && c0 != '\'')
			break;
		val += c0;
	}
	src.push(c0);
	return num_token(val);
}

// Synchronize the input line number with the output line
void
TokenizerBase::lines_synchronize()
{
	while (src.line_number() > output_line_number) {
		std::cout << std::endl;
		output_line_number++;
	}
}

/*
 * Output numeric token values.
 * If compress is true, all identifiers, numbers, and types have the same value.
 */
void
TokenizerBase::numeric_tokenize(bool compress)
{
	token_type c;

	previously_in_method = false;
	while ((c = get_token())) {

		if (compress) {
			// Merge together a series of type tokens
			if (keyword.is_type(c)) {
				token_type c2 = get_token();
				push_token(c2);
				if (keyword.is_type(c2))
					continue; // Ignore c
			}

			if (TokenId::is_identifier(c))
				c = TokenId::ANY_IDENTIFIER;
			else if (keyword.is_type(c))
				c = TokenId::ANY_TYPE;
			else if (TokenId::is_number(c))
				c = TokenId::ANY_NUMBER;
			else if (TokenId::is_hashed_content(c))
				c = TokenId::ANY_HASH;
		}

		switch (processing_type) {
		case PT_LINE:
			lines_synchronize();
			// FALLTHROUGH
		case PT_FILE:
			std::cout << c << separator;
			break;
		case PT_METHOD:
			if (previously_in_method && !nesting.in_method())
				std::cout << c << std::endl;
			if (nesting.in_method())
				std::cout << c << separator;
			break;
		case PT_STATEMENT:
			if (previously_in_method && !nesting.in_method())
				std::cout << c << std::endl;
			if (nesting.in_method()) {
				if (c == ';')
					std::cout << c << std::endl;
				else
					std::cout << c << separator;
			}
			break;
		}
		previously_in_method = nesting.in_method();
	}

	std::cout << std::endl;
}

void
TokenizerBase::type_tokenize()
{
	token_type c;

	previously_in_method = false;
	while ((c = get_token())) {
		std::ostringstream os;

		if (TokenId::is_character(c))
			os << (char)c;
		else if (TokenId::is_keyword(c))
			os.str(keyword_to_string(c));
		else if (TokenId::is_other_token(c))
			os.str(token_to_string(c));
		else if (TokenId::is_zero(c) || TokenId::is_number(c))
			os << "NUM";
		else if (TokenId::is_identifier(c))
			os << "ID";
		else if (TokenId::is_hashed_content(c))
			os << "HASH";
		else
			assert(false);

		delimit(os.str(), c);
	}

	std::cout << std::endl;
}


/*
 * Output a token c named s preceded or followed by any
 * required delimiters.
 */
void
TokenizerBase::delimit(const std::string &s, token_type c)
{
	switch (processing_type) {
	case PT_LINE:
		lines_synchronize();
		// FALLTHROUGH
	case PT_FILE:
		std::cout << s << separator;
		break;
	case PT_METHOD:
		if (previously_in_method && !nesting.in_method())
			std::cout << s << std::endl;
		if (nesting.in_method())
			std::cout << s << separator;
		break;
	case PT_STATEMENT:
		if (previously_in_method && !nesting.in_method())
			std::cout << s << std::endl;
		if (nesting.in_method()) {
			if (c == ';')
				std::cout << s << std::endl;
			else
				std::cout << s << separator;
		}
		break;
	}
	previously_in_method = nesting.in_method();
}

void
TokenizerBase::symbolic_tokenize()
{
	token_type c;

	previously_in_method = false;
	while ((c = get_token())) {
		std::ostringstream os;

		if (TokenId::is_character(c))
			os << (char)c;
		else if (TokenId::is_keyword(c))
			os.str(keyword_to_string(c));
		else if (TokenId::is_other_token(c))
			os.str(token_to_string(c));
		else if (TokenId::is_zero(c))
			os.str("0");
		else if (TokenId::is_number(c))
			os << "~1E" << c - TokenId::NUMBER_ZERO;
		else if (TokenId::is_identifier(c))
			os << "ID:" << c;
		else if (TokenId::is_hashed_content(c))
			os << "HASH:" << c;
		else
			assert(false);

		delimit(os.str(), c);
	}

	std::cout << std::endl;
}

void
TokenizerBase::code_tokenize()
{
	token_type c;

	while ((c = get_token())) {
		if (TokenId::is_character(c) && !isspace((unsigned char)c))
			std::cout << (char)c;
		else if (TokenId::is_keyword(c))
			std::cout <<keyword_to_string(c);
		else if (TokenId::is_other_token(c))
			std::cout << token_to_symbol(c);
		else if (TokenId::is_zero(c))
			std::cout << "0";
		else if (TokenId::is_number(c))
			std::cout << get_value();
		else if (TokenId::is_identifier(c))
			std::cout << get_value();
		else if (TokenId::is_hashed_content(c))
			std::cout << get_value();
		else
			assert(false);
		std::cout << std::endl;
	}
}

void
TokenizerBase::type_code_tokenize()
{
	token_type c;

	while ((c = get_token())) {
		if (TokenId::is_character(c) && !isspace((unsigned char)c))
			std::cout << "TOK " << (char)c;
		else if (TokenId::is_keyword(c))
			std::cout << "KW " << keyword_to_string(c);
		else if (TokenId::is_other_token(c))
			std::cout << "TOK " << token_to_symbol(c);
		else if (TokenId::is_zero(c))
			std::cout << "NUM 0";
		else if (TokenId::is_number(c))
			std::cout << "NUM " << get_value();
		else if (TokenId::is_identifier(c))
			std::cout << "ID " << get_value();
		else if (TokenId::is_hashed_content(c))
			std::cout << "HASH " << get_value();
		else
			assert(false);
		std::cout << std::endl;
	}
}

void
TokenizerBase::process_options(std::vector<std::string> opt)
{
	for (auto &o : opt) {
		if (o == "file")
			processing_type = PT_FILE;
		else if (o == "line")
			processing_type = PT_LINE;
		else if (o == "method")
			processing_type = PT_METHOD;
		else if (o == "statement")
			processing_type = PT_STATEMENT;
		else {
			std::cerr << "Unsupported processing option [" << o <<
				"]" << std::endl;
			std::cerr << "Valid options are one of file, line, method, statement" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

// Return a single token from the queue or the lexical stream
token_type
TokenizerBase::get_token()
{
	if (token_queue.empty())
		return get_immediate_token();

	token_type token = token_queue.back();
	token_queue.pop_back();
	return token;
}

TokenizerBase::~TokenizerBase()
{
}
