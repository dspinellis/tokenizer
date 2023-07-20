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

#ifndef PHPTOKENIZER_H
#define PHPTOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "Keyword.h"
#include "Token.h"
#include "TokenizerBase.h"

/** Split input into language-specific tokens */
class PHPTokenizer : public TokenizerBase {
private:
	Keyword php_keyword;
	Token php_token;
	bool process_here_document();
public:
	token_type get_token();		// Return a single token

	const std::string & keyword_to_string(token_type k) const {
		return php_keyword.to_string(k);
	}

	const std::string & token_to_string(token_type k) const {
		return php_token.to_string(k);
	}

	const std::string & token_to_symbol(token_type k) const {
		return php_token.to_symbol(k);
	}

	// Construct from a character source
	PHPTokenizer(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		TokenizerBase(s, file_name, opt), php_keyword(Keyword::L_PHP) {}

	// Construct for a string source
	PHPTokenizer(const std::string &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s, opt), php_keyword(Keyword::L_PHP) {}

	~PHPTokenizer();

	friend class PHPTokenizerTest;
};
#endif /* PHPTOKENIZER_H */
