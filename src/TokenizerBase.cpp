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
#include "TokenId.h"

/*
 * Convert the passed number into an integer token value, in the range
 * NUMBER_ZERO +- 500, with 0 being NUMBER_ZERO and the rest being
 * represented through base 10 logarithms.
 */
int
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

	d = log(d) / log(10);
	if (d >= 0)
		d = ceil(d) + 1;
	if (d < 0)
		d = floor(d);
	return TokenId::NUMBER_ZERO + d;
}

// Process a block comment, returning false on EOF
bool
TokenizerBase::process_block_comment()
{
	char c1;

	src.get(c1);
	for (;;) {
		while (c1 != '*') {
			if (!isspace(c1) && bol.at_bol_space())
				bol.saw_non_space();
			if (!src.get(c1)) {
				error("EOF encountered while processing a block comment");
				return false;
			}
		}
		if (!isspace(c1) && bol.at_bol_space())
			bol.saw_non_space();
		if (!src.get(c1)) {
			error("EOF encountered while processing a block comment");
			return false;
		}
		if (c1 == '/')
			break;
	}
	return true;
}

// Process a line comment, returning false on EOF
bool
TokenizerBase::process_line_comment()
{
	char c1;

	src.get(c1);
	for (;;) {
		if (c1 == '\n')
			break;
		if (!src.get(c1))
			return false;
	}
	src.push(c1);
	return true;
}

// Process a character literal, returning false on EOF
bool
TokenizerBase::process_char_literal()
{
	char c0;

	for (;;) {
		if (!src.get(c0)) {
			error("EOF encountered while processing a character literal");
			return false;
		}
		if (c0 == '\\') {
			// Consume one character after the backslash
			// ... to deal with the '\'' problem
			src.get(c0);
			continue;
		}
		if (c0 == '\'')
			break;
	}
	return true;
}

// Process a string literal, returning false on EOF
bool
TokenizerBase::process_string_literal()
{
	char c0;

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
		} else if (c0 == '"')
			break;
	}
	return true;
}

// Process a number starting with the passed string returning its token value
int
TokenizerBase::process_number(std::string &val)
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
		if (!isalnum(c0) && c0 != '.' && c0 != '_')
			break;
		val += c0;
	}
	src.push(c0);
	return num_token(val);
}

void
TokenizerBase::numeric_tokenize()
{
	int c;

	previously_in_method = false;
	while ((c = get_token())) {
		switch (processing_type) {
		case PT_FILE:
			std::cout << c << '\t';
			break;
		case PT_METHOD:
			if (previously_in_method && !nesting.in_method())
				std::cout << c << std::endl;
			if (nesting.in_method())
				std::cout << c << '\t';
			break;
		case PT_STATEMENT:
			if (previously_in_method && !nesting.in_method())
				std::cout << c << std::endl;
			if (nesting.in_method()) {
				if (c == ';')
					std::cout << c << std::endl;
				else
					std::cout << c << '\t';
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
	int c;

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
		else
			assert(false);

		delimit(os.str(), c);
	}

	std::cout << std::endl;
}


void
TokenizerBase::delimit(const std::string &s, int c)
{
	switch (processing_type) {
	case PT_FILE:
		std::cout << s << ' ';
		break;
	case PT_METHOD:
		if (previously_in_method && !nesting.in_method())
			std::cout << s << std::endl;
		if (nesting.in_method())
			std::cout << s << ' ';
		break;
	case PT_STATEMENT:
		if (previously_in_method && !nesting.in_method())
			std::cout << s << std::endl;
		if (nesting.in_method()) {
			if (c == ';')
				std::cout << s << std::endl;
			else
				std::cout << s << ' ';
		}
		break;
	}
	previously_in_method = nesting.in_method();
}

void
TokenizerBase::symbolic_tokenize()
{
	int c;

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
		else
			assert(false);

		delimit(os.str(), c);
	}

	std::cout << std::endl;
}

void
TokenizerBase::code_tokenize()
{
	int c;

	while ((c = get_token())) {
		if (TokenId::is_character(c) & !isspace((unsigned char)c))
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
		else
			assert(false);
		std::cout << std::endl;
	}
}

void
TokenizerBase::type_code_tokenize()
{
	int c;

	while ((c = get_token())) {
		if (TokenId::is_character(c) & !isspace((unsigned char)c))
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
		else if (o == "method")
			processing_type = PT_METHOD;
		else if (o == "statement")
			processing_type = PT_STATEMENT;
		else {
			std::cerr << "Unsupported processing option [" << o <<
				"]" << std::endl;
			std::cerr << "Valid options are one of file, method, statement" << std::endl;
			exit(1);
		}
	}
}

TokenizerBase::~TokenizerBase()
{
}
