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

#ifndef JAVATOKENIZER_H
#define JAVATOKENIZER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "JavaKeyword.h"
#include "TokenizerBase.h"

/** Collect quality metrics from C-like source code */
class JavaTokenizer : public TokenizerBase {
private:
	JavaKeyword java_keyword;
	enum ProcessingType {
		PT_CLASS,		// Output vector for whole class
		PT_METHOD,		// Output vector for each method
		PT_STATEMENT,		// Output vector for each statement
	} processing_type;

	void process_options(std::vector<std::string> opt);

	enum ProcessingType get_processing_type() const {
		return processing_type;
	}
public:
	int get_token();		// Return a single token

	// Construct from a character source
	JavaTokenizer(CharSource &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s), processing_type(PT_CLASS) {
		process_options(opt);
	}

	// Construct for a string source
	JavaTokenizer(const std::string &s, std::vector<std::string> opt = {}) :
		TokenizerBase(s), processing_type(PT_CLASS) {
		process_options(opt);
	}

	~JavaTokenizer();

	friend class JavaTokenizerTest;
};
#endif /* JAVATOKENIZER_H */
