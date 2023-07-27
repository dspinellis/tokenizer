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

#ifndef JAVASCRIPTTOKENIZER_H
#define JAVASCRIPTTOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "Keyword.h"
#include "Token.h"
#include "TokenizerBase.h"

/** Split input into language-specific tokens */
class JavaScriptTokenizer : public TokenizerBase {
private:
	Token javascript_token;
	bool expression_context;  // When true / and /= are division operators
	bool process_string_literal(char c);
	bool process_regex_literal();
	// Get a token, knowing the current context
	inline token_type get_token_for_context();
public:
	token_type get_immediate_token();		// Return a single token

	const std::string & keyword_to_string(token_type k) const {
		return keyword.to_string(k);
	}

	const std::string & token_to_string(token_type k) const {
		return javascript_token.to_string(k);
	}

	const std::string & token_to_symbol(token_type k) const {
		return javascript_token.to_symbol(k);
	}

	// Construct from a character source
	JavaScriptTokenizer(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {},
			Keyword::LanguageId kw_id = Keyword::L_JavaScript) :
		TokenizerBase(kw_id, s, file_name, opt),
		expression_context(false) {}

	// Construct for a string source
	JavaScriptTokenizer(const std::string &s,
			std::vector<std::string> opt = {},
			Keyword::LanguageId kw_id = Keyword::L_JavaScript) :
		TokenizerBase(kw_id, s, opt),
		expression_context(false) {}

	~JavaScriptTokenizer();

	friend class JavaScriptTokenizerTest;
	friend class TypeScriptTokenizerTest;
};
#endif /* JAVASCRIPTTOKENIZER_H */
