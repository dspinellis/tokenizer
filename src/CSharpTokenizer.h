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

#ifndef CSHARPTOKENIZER_H
#define CSHARPTOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "CSharpKeyword.h"
#include "TokenizerBase.h"
#include "SymbolTable.h"
#include "NestedClassState.h"

/** Collect quality metrics from C-like source code */
class CSharpTokenizer : public TokenizerBase {
private:
	CSharpKeyword java_keyword;
	enum ProcessingType {
		PT_FILE,		// Output vector for whole class
		PT_METHOD,		// Output vector for each method
		PT_STATEMENT,		// Output vector for each statement
	} processing_type;

	void process_options(std::vector<std::string> opt);

	enum ProcessingType get_processing_type() const {
		return processing_type;
	}
	SymbolTable symbols;
	NestedClassState nesting;
	int get_token_real();		// Return a single token
	int previous_token;		// Previously returned token
public:
	// Return a single token coalescing together multiple line doc comments
	int get_token();

	// Construct from a character source
	CSharpTokenizer(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		TokenizerBase(s, file_name), processing_type(PT_FILE) {
		process_options(opt);
	}

	// Construct for a string source
	CSharpTokenizer(const std::string &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s), processing_type(PT_FILE) {
		process_options(opt);
	}

	void tokenize();

	~CSharpTokenizer();

	friend class CSharpTokenizerTest;
};
#endif /* CSHARPTOKENIZER_H */
