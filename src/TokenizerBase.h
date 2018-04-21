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

#ifndef TOKENIZERBASE_H
#define TOKENIZERBASE_H

#include <iostream>
#include <sstream>
#include <vector>

#include "CharSource.h"
#include "CKeyword.h"
#include "BolState.h"
#include "SymbolTable.h"
#include "NestedClassState.h"

/** Collect quality metrics from C-like source code */
class TokenizerBase {
protected:
	std::stringstream string_src;	// Source for testing
	CharSource src;			// Character source
	/** True for keywords that don't end with semicolon */
	bool saw_comment;		// True after a comment
	/** Called at every encountered newline */
	void newline(bool in_non_code_block = false) { line_number++; }
	BolState bol;			// Beginning of line state
	std::string input_file;		// Input file name
	int line_number;		// Input line number
	// Report an error message
	void error(const std::string &msg) {
		std::cerr << input_file << '(' << line_number << "): " <<
			msg << std::endl;
	}
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
public:
	virtual int get_token() = 0;	// Return a single token
	virtual const std::string & keyword_to_string(int k) const = 0;
	virtual const std::string & token_to_string(int k) const = 0;
	void tokenize();		// Tokenize numbers to stdout
	void symbolic_tokenize();	// Tokenize symbols to stdout

	// Construct from a character source
	TokenizerBase(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		src(s), saw_comment(false), input_file(file_name),
		line_number(1), processing_type(PT_FILE) {
		process_options(opt);
	}

	// Construct for a string source
	TokenizerBase(const std::string &s,
			std::vector<std::string> opt = {}) :
		string_src(s), src(string_src), saw_comment(false),
		input_file("(string)"), line_number(1),
		processing_type(PT_FILE) {
		process_options(opt);
	}

	~TokenizerBase();

	static int num_token(const std::string &val);
	bool process_block_comment();
	bool process_line_comment();
	bool process_char_literal();
	bool process_string_literal();
	int process_number(std::string &val);
};
#endif /* TOKENIZERBASE_H */
