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
 * Calculate the hash value of an accumulated string
 */

#pragma once

#include <string>
#include <functional>

#include "TokenId.h"

class IncrementalHash {
	static const size_t RESERVE_SIZE = 4096;

	std::string buffer;
	std::hash<std::string> hasher;
public:
	IncrementalHash() {
		buffer.reserve(RESERVE_SIZE);
	}

	void add(char c) {
		buffer.push_back(c);
	}

	void add(token_type t) {
		buffer.append(std::to_string(t));
	}

	token_type get() {
		return static_cast<token_type>(hasher(buffer)) | TokenId::HASHED_CONTENT ;
	}

	void reset() {
		buffer.clear();
	}
};
