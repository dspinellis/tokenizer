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
#include "TokenizerBase.h"

/*
 * Convert the passed number into an integer token value, in the range
 * 1000-2000, with 0 being 1500 and the rest being represented through
 * base 10 logarithms.
 */
int
TokenizerBase::num_token(const std::string &val)
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
			if (!src.get(c1))
				return false;
			if (c1 == '\n')
				newline(true);
		}
		if (!isspace(c1) && bol.at_bol_space())
			bol.saw_non_space();
		if (!src.get(c1))
			return false;
		if (c1 == '/')
			break;
		else if (c1 == '\n')
				newline();
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
		if (!src.get(c0))
			return false;
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
		if (!src.get(c0))
			return false;
		if (c0 == '\\') {
			// Consume one character after the backslash
			src.get(c0);
			if (c0 == '\n')
				newline(true);
			continue;
		} else if (c0 == '"')
			break;
		else if (c0 == '\n')
			newline(true);
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
TokenizerBase::tokenize()
{
	int c;

	while ((c = get_token()))
		std::cout << c << ' ';
	std::cout << std::endl;
}

TokenizerBase::~TokenizerBase()
{
}
