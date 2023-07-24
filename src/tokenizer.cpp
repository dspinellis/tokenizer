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

#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <optional>
#include <vector>

#include "errno.h"
#include "unistd.h"

#include "SymbolTable.h"
#include "CTokenizer.h"
#include "CppTokenizer.h"
#include "CSharpTokenizer.h"
#include "GoTokenizer.h"
#include "JavaTokenizer.h"
#include "JavaScriptTokenizer.h"
#include "PHPTokenizer.h"
#include "PythonTokenizer.h"
#include "RustTokenizer.h"

const char version[] = "2.8.1";

// Command-line option values
static bool all_contents = false;
static bool symbolic_output = false;
static bool compress_ids = false;
static bool show_file_name = false;
static enum output_type {
	ot_tokens,	// Numeric or symbolic tokens
	ot_break, 	// Original tokens broken into lines
	ot_type_break,	// As above, tokens preceded by their type
} output_type = ot_tokens;
static std::string lang("Java");
static std::vector<std::string> processing_opt;
static char separator;

/*
 * Process and print the metrics of the specified stream,
 * which is identified with the specified filename.
 */
static void
process_file(std::istream &in, std::string filename)
{
	CharSource cs(in);
	TokenizerBase *t;

	if (lang == "C")
		t = new CTokenizer(cs, filename, processing_opt);
	else if (lang == "CSharp" || lang == "C#")
		t = new CSharpTokenizer(cs, filename, processing_opt);
	else if (lang == "Go")
		t = new GoTokenizer(cs, filename, processing_opt);
	else if (lang == "Java")
		t = new JavaTokenizer(cs, filename, processing_opt);
	else if (lang == "JavaScript")
		t = new JavaScriptTokenizer(cs, filename, processing_opt);
	else if (lang == "C++")
		t = new CppTokenizer(cs, filename, processing_opt);
	else if (lang == "PHP")
		t = new PHPTokenizer(cs, filename, processing_opt);
	else if (lang == "Python")
		t = new PythonTokenizer(cs, filename, processing_opt);
	else if (lang == "Rust")
		t = new RustTokenizer(cs, filename, processing_opt);
	else if (lang == "TypeScript")
		t = new JavaScriptTokenizer(cs, filename, processing_opt,
				Keyword::L_TypeScript);
	else {
		std::cerr << "Unknown language specified." << std::endl;
		std::cerr << "The following languages are supported:" << std::endl;
		std::cerr << "\tC" << std::endl;
		std::cerr << "\tCSharp (or C#)" << std::endl;
		std::cerr << "\tC++" << std::endl;
		std::cerr << "\tGo" << std::endl;
		std::cerr << "\tJava" << std::endl;
		std::cerr << "\tJavaScript" << std::endl;
		std::cerr << "\tPHP" << std::endl;
		std::cerr << "\tPython" << std::endl;
		std::cerr << "\tRust" << std::endl;
		std::cerr << "\tTypeScript" << std::endl;
		exit(EXIT_FAILURE);
	}

	t->set_separator(separator ? separator : ' ');
	t->set_all_contents(all_contents);
	switch (output_type) {
	case ot_tokens:
		if (symbolic_output) {
			if (compress_ids)
				t->type_tokenize();
			else
				t->symbolic_tokenize();
		} else {
			t->set_separator(separator ? separator : '\t');
			t->numeric_tokenize(compress_ids);
		}
		break;
	case ot_type_break:
		t->type_code_tokenize();
		break;
	case ot_break:
		t->code_tokenize();
		break;
	}
}

// List the values of all tokens
static void
list_tokens()
{
	// Characters (ASCII assumed)
	for (char c = ' '; c <= '~'; c++)
		std::cout << static_cast<int>(c) << '\t' << c << std::endl;

	// Character symbol tokens (e.g. +=)
	Token t;
	for (auto ti: t.token_symbol_view())
		std::cout << ti.first << '\t' << ti.second << std::endl;

	// Keywords (e.g. "if")
	// Language is irrelevant for token_keyword_view
	Keyword k(Keyword::L_C);
	for (auto ki: k.token_keyword_view())
		std::cout << ki.first << '\t' << ki.second << std::endl;

	// Compressed token identifiers
	std::cout << TokenId::ANY_HASH << "\tANY_HASH" << std::endl;
	std::cout << TokenId::ANY_NUMBER << "\tANY_NUMBER" << std::endl;
	std::cout << TokenId::ANY_IDENTIFIER << "\tANY_IDENTIFIER" << std::endl;

	// Numbers
	for (long double d = 1e-308L; d < 1; d *= 10)
		std::cout << TokenizerBase::compress(static_cast<double>(d)) << '\t' << d << std::endl;
	std::cout << TokenId::NUMBER_ZERO << "\t0" << std::endl;
	for (long double d = 1; d < 1e309L; d *= 10)
		std::cout << TokenizerBase::compress(static_cast<double>(d)) << '\t' << d << std::endl;

	// RLE horizontal space
	for (int i = 2; i <= TokenId::RLE_MAX; ++i)
		std::cout << TokenId::RLE_SPACE + i << '\t' << "' ' * " << i << std::endl;
	for (int i = 2; i <= TokenId::RLE_MAX; ++i)
		std::cout << TokenId::RLE_TAB + i << '\t' << "'\\t' * " << i << std::endl;

	// Other token values
	std::cout << TokenId::NUMBER_INFINITE << "\tINFINITE" << std::endl;
	std::cout << TokenId::NUMBER_NAN << "\tNAN" << std::endl;
	std::cout << TokenId::FIRST_IDENTIFIER << "\tFIRST_IDENTIFIER" << std::endl;
	std::cout << TokenId::HASHED_CONTENT << "\tFIST_HASHED_CONTENT" << std::endl;
}

// Open and process the specified file
static void
process_named_file(std::string filename)
{
	std::ifstream in;

	in.open(filename, std::ifstream::in);
	if (!in.good()) {
		std::cerr << "Unable to open " << filename <<
			": " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	if (show_file_name)
		std::cout << "F" << filename << std::endl;
	process_file(in, filename);
	in.close();
}

// Process the files listed in the specified input stream
static void
process_files_from_stream(std::istream &in)
{
	std::string filename;

	while (std::getline(in, filename))
		process_named_file(filename);
}

/* Read code and output its constituent tokens */
int
main(int argc, char * const argv[])
{
	int opt;
	std::optional<std::string> files_list(std::nullopt);

	while ((opt = getopt(argc, argv, "aBbcfgi:Ll:o:st:V")) != -1)
		switch (opt) {
		case 'a':
			all_contents = true;
			break;
		case 'B':
			output_type = ot_type_break;
			break;
		case 'b':
			output_type = ot_break;
			break;
		case 'c':
			compress_ids = true;
			break;
		case 'f':
			show_file_name = true;
			break;
		case 'g':
			SymbolTable::disable_scoping();
			break;
		case 'i':
			files_list = optarg;
			break;
		case 'L':
			list_tokens();
			exit(EXIT_SUCCESS);
			break;
		case 'l':
			lang = optarg;
			break;
		case 'o':
			processing_opt.push_back(optarg);
			break;
		case 's':
			symbolic_output = true;
			break;
		case 't':
			separator = *optarg;
			break;
		case 'V':
			std::cout << "tokenizer " << version << std::endl;
			exit(EXIT_SUCCESS);
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				"  [-acgs | -B | -b] [-fV] [-i file] [-l lang] [-o opt] [-t sep] [file ...]" << std::endl;
			exit(EXIT_FAILURE);
		}

	if (argv[optind] && files_list.has_value()) {
		std::cerr << "Specify either an input file list or"
			" command-line arguments; not both." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Process tokens from standard input
	if (!argv[optind] && !files_list.has_value()) {
		process_file(std::cin, "-");
		exit(EXIT_SUCCESS);
	}

	if (files_list.has_value()) {

		if (files_list.value() == "-") {
			process_files_from_stream(std::cin);
		} else {
			std::ifstream file_list_stream(files_list.value());
			if (!file_list_stream) {
				std::cerr << "Unable to open "
					<< files_list.value() <<
					": " << strerror(errno) << std::endl;
				exit(EXIT_FAILURE);
			}
			process_files_from_stream(file_list_stream);
		}

	}

	// Read from files specified as arguments
	for (; argv[optind]; optind++)
		process_named_file(argv[optind]);

	exit(EXIT_SUCCESS);
}
