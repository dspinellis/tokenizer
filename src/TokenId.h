/*-
 * Copyright 2018 Diomidis Spinellis
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

#ifndef TOKEIND_H
#define TOKEIND_H

/**
 * Numeric identifiers used for representing tokens.
 */
class TokenId {
public:
	// 0-255 ASCII characters
	static constexpr int CHARACTER = 0;

	// Language's keywords
	static constexpr int KEYWORD = 300;

	// Multi-character operators and other non-keyword tokens (e.g. elipsis)
	static constexpr int OTHER_TOKEN = 600;

	// Numbers on a log_10 scale are centered around this value
	// and extend +-400 around it
	static constexpr int NUMBER_START = 1100;
	static constexpr int NUMBER_ZERO = 1500;
	static constexpr int NUMBER_INFINITE = 1900;
	static constexpr int NUMBER_NAN = 1901;
	static constexpr int NUMBER_END = 1902;

	// Identifiers are dynamically allocated from this number upward
	static constexpr int IDENTIFIER = 2000;

	static bool is_character(int t) { return t < KEYWORD; }
	static bool is_keyword(int t) { return t >= KEYWORD && t < OTHER_TOKEN; }
	static bool is_other_token(int t) { return t >= OTHER_TOKEN && t < NUMBER_START; }
	static bool is_zero(int t) { return t == NUMBER_ZERO; }
	static bool is_number(int t) { return t >= NUMBER_START && t < NUMBER_END; }
	static bool is_identifier(int t) { return t >= IDENTIFIER; }
};
#endif /* TOKEIND_H */
