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
#include "Keyword.h"
#include "BolState.h"
#include "SymbolTable.h"
#include "NestedClassState.h"

/** Collect quality metrics from C-like source code */
class TokenizerBase {
private:
	bool previously_in_method;
	void delimit(const std::string &s, int c);
protected:
	std::stringstream string_src;	// Source for testing
	CharSource src;			// Character source
	/** True for keywords that don't end with semicolon */
	bool saw_comment;		// True after a comment
	BolState bol;			// Beginning of line state
	std::string input_file;		// Input file name
	std::string val;		// Token value (ids, strings, nums, ...)
	// Report an error message
	void error(const std::string &msg) {
		std::cerr << input_file << '(' << src.line_number() << "): " <<
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
	virtual const std::string & token_to_symbol(int k) const = 0;
	void numeric_tokenize();	// Tokenize numbers to stdout
	void symbolic_tokenize();	// Tokenize symbols to stdout
	void code_tokenize();		// Tokenize code to stdout
	void type_tokenize();		// Tokenize token types to stdout
	void type_code_tokenize();	// Tokenize token code and its type to stdoit

	// Construct from a character source
	TokenizerBase(CharSource &s, const std::string &file_name,
			std::vector<std::string> opt = {}) :
		src(s), saw_comment(false), input_file(file_name),
		processing_type(PT_FILE) {
		process_options(opt);
	}

	// Construct for a string source
	TokenizerBase(const std::string &s,
			std::vector<std::string> opt = {}) :
		string_src(s), src(string_src), saw_comment(false),
		input_file("(string)"),
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
	const std::string & get_value() const { return val; }
};
#endif /* TOKENIZERBASE_H */
