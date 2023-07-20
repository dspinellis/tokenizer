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

#ifndef CPPTOKENIZER_H
#define CPPTOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "Keyword.h"
#include "Token.h"
#include "TokenizerBase.h"

/** Split input into language-specific tokens */
class CppTokenizer : public TokenizerBase {
private:
	bool scan_cpp_directive;	// Keyword after a C preprocessor #
	Keyword cpp_keyword;
	Token cpp_token;
public:
	token_type get_immediate_token();		// Return a single token

	const std::string & keyword_to_string(token_type k) const {
		return cpp_keyword.to_string(k);
	}

	const std::string & token_to_string(token_type k) const {
		return cpp_token.to_string(k);
	}

	const std::string & token_to_symbol(token_type k) const {
		return cpp_token.to_symbol(k);
	}

	// Construct from a character source
	CppTokenizer(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		TokenizerBase(s, file_name, opt), scan_cpp_directive(false),
		cpp_keyword(Keyword::L_Cpp) {}

	// Construct for a string source
	CppTokenizer(const std::string &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s, opt), scan_cpp_directive(false),
		cpp_keyword(Keyword::L_Cpp) {}

	~CppTokenizer();

	friend class CppTokenizerTest;
};
#endif /* CPPTOKENIZER_H */
