/*-
 * Copyright 2023 Diomidis Spinellis
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *	   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 * Run length encode space and tab sequences
 */

#pragma once

#include "TokenId.h"

class RunLengthEncoder {
	char previous;  // Previously encountered tab or space
	int count;	// Number of encounters
	CharSource &src;// Character source for pushing deferred chars

	// Return the run-length encoding of the stored data
	token_type rle_token() {
		if (count == 1)
			return static_cast<token_type>(previous);
		else if (previous == ' ')
			return TokenId::RLE_SPACE + count;
		else
			return TokenId::RLE_TAB + count;
	}
public:
	RunLengthEncoder(CharSource &s) : previous(0), count(0), src(s) {}

	/*
	 * Given an input character, return either a token value to return
	 * from the tokenizer or 0 to continue the tokenizer's processing.
	 *
	 * This implements the following decision table.
	 * (hspace means horizontal space: ' ' or '\t'.)
	 *
	 *          input: hspace              other
	 * state:
	 * initial         store c             return process
	 *                 count = 1
	 *                 return process
	 *
	 * same hspace     ++count             \
	 * count < max     return process       |
	 *                                      |
	 * same hspace     count = 1            | push c
	 * count == max    return RLE           | state = init
	 *                                      | return RLE
	 * other hspace    store c              |
	 *                 count = 1            |
	 *                 return RLE          /
	 */
	token_type add(char c) {
		token_type ret;

		// Horizontal space
		if (c == ' ' || c == '\t') {
			if (previous == 0) {
				previous = c;
				count = 1;
				ret = 0;
			} else if (c == previous) {
				if (count < TokenId::RLE_MAX) {
					++count;
					ret = 0;
				} else {
					ret = rle_token();
					count = 1;
				}
			} else {
				ret = rle_token();
				previous = c;
				count = 1;
			}
		} else {
			if (previous == 0)
				ret = 0;
			else {
				ret = rle_token();
				src.push(c);
				previous = 0;
			}
		}
		return ret;
	}
};
