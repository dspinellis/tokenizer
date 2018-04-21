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

#ifndef CPPTOKENIZER_H
#define CPPTOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "CppKeyword.h"
#include "TokenizerBase.h"

/** Collect quality metrics from C-like source code */
class CppTokenizer : public TokenizerBase {
private:
	CppKeyword java_keyword;
public:
	int get_token();		// Return a single token

	// Construct from a character source
	CppTokenizer(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		TokenizerBase(s, file_name) {
		process_options(opt);
	}

	// Construct for a string source
	CppTokenizer(const std::string &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s) {
		process_options(opt);
	}

	~CppTokenizer();

	friend class CppTokenizerTest;
};
#endif /* CPPTOKENIZER_H */
