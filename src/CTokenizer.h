/*-
 * Copyright 2014 Diomidis Spinellis
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

#ifndef CTOKENIZER_H
#define CTOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "CKeyword.h"
#include "CToken.h"
#include "TokenizerBase.h"

/** Collect quality metrics from C-like source code */
class CTokenizer : public TokenizerBase {
private:
	bool scan_cpp_directive;	// Keyword after a C preprocessor #
	bool scan_cpp_line;		// Line after a C preprocessor #
	/** True for keywords that don't end with semicolon */
	CKeyword ckeyword;
	CToken ctoken;
public:
	int get_token();		// Return a single token

	const std::string & keyword_to_string(int k) const {
		return ckeyword.to_string(k);
	}

	const std::string & token_to_string(int k) const {
		return ctoken.to_string(k);
	}

	const std::string & token_to_symbol(int k) const {
		return ctoken.to_symbol(k);
	}

	// Construct from a character source
	CTokenizer(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		TokenizerBase(s, file_name, opt), scan_cpp_directive(false),
		scan_cpp_line(false) { }

	// Construct for a string source
	CTokenizer(const std::string &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s, opt), scan_cpp_directive(false),
		scan_cpp_line(false) {}

	~CTokenizer();
};
#endif /* CTOKENIZER_H */
