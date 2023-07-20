/*-
 * Copyright 2018-2023 Diomidis Spinellis
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

typedef unsigned int token_type;

/**
 * Numeric identifiers used for representing tokens.
 */
class TokenId {
public:
	// 0-255 ASCII characters
	static constexpr token_type CHARACTER = 0;

	// Language's keywords
	static constexpr token_type KEYWORD = 300;

	// Multi-character operators and other non-keyword tokens (e.g. elipsis)
	static constexpr token_type OTHER_TOKEN = 600;

	// Numbers on a log_10 scale are centered around this value
	// and extend +-400 around it
	static constexpr token_type NUMBER_START = 1100;
	static constexpr token_type NUMBER_ZERO = 1500;
	static constexpr token_type NUMBER_INFINITE = 1900;
	static constexpr token_type NUMBER_NAN = 1901;
	static constexpr token_type NUMBER_END = 1902;

	// Identifiers are dynamically allocated from this number upward
	static constexpr token_type IDENTIFIER = 2000;

	// Numbers and identifiers compressed to a single value
	static constexpr token_type COMPRESSED_NUMBER = 698;
	static constexpr token_type COMPRESSED_IDENTIFIER = 699;

	static bool is_character(token_type t) { return t < KEYWORD; }
	static bool is_keyword(token_type t) { return t >= KEYWORD && t < OTHER_TOKEN; }
	static bool is_other_token(token_type t) { return t >= OTHER_TOKEN && t < NUMBER_START; }
	static bool is_zero(token_type t) { return t == NUMBER_ZERO; }
	static bool is_number(token_type t) { return t >= NUMBER_START && t < NUMBER_END; }
	static bool is_identifier(token_type t) { return t >= IDENTIFIER; }
};
#endif /* TOKEIND_H */
