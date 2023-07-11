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
#include "JavaTokenizer.h"
#include "JavaScriptTokenizer.h"
#include "PHPTokenizer.h"
#include "PythonTokenizer.h"

const char version[] = "2.4.0";

// Command-line option values
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

/*
 * Process and print the metrics of the specified stream,
 * which is identified with the specified filename.
 */
static void
process_file(std::istream &in, std::string filename)
{
	CharSource cs(in);
	TokenizerBase *t;

	if (lang == "Java")
		t = new JavaTokenizer(cs, filename, processing_opt);
	else if (lang == "JavaScript")
		t = new JavaScriptTokenizer(cs, filename, processing_opt);
	else if (lang == "C")
		t = new CTokenizer(cs, filename, processing_opt);
	else if (lang == "CSharp" || lang == "C#")
		t = new CSharpTokenizer(cs, filename, processing_opt);
	else if (lang == "C++")
		t = new CppTokenizer(cs, filename, processing_opt);
	else if (lang == "PHP")
		t = new PHPTokenizer(cs, filename, processing_opt);
	else if (lang == "Python")
		t = new PythonTokenizer(cs, filename, processing_opt);
	else {
		std::cerr << "Unknown language specified." << std::endl;
		std::cerr << "The following languages are supported:" << std::endl;
		std::cerr << "\tC" << std::endl;
		std::cerr << "\tCSharp (or C#)" << std::endl;
		std::cerr << "\tC++" << std::endl;
		std::cerr << "\tJava" << std::endl;
		std::cerr << "\tJavaScript" << std::endl;
		std::cerr << "\tPHP" << std::endl;
		std::cerr << "\tPython" << std::endl;
		exit(EXIT_FAILURE);
	}

	switch (output_type) {
	case ot_tokens:
		if (symbolic_output) {
			if (compress_ids)
				t->type_tokenize();
			else
				t->symbolic_tokenize();
		} else
			t->numeric_tokenize(compress_ids);
		break;
	case ot_type_break:
		t->type_code_tokenize();
		break;
	case ot_break:
		t->code_tokenize();
		break;
	}
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

/* Calculate and print C metrics for the standard input */
int
main(int argc, char * const argv[])
{
	int opt;
	std::optional<std::string> files_list(std::nullopt);

	while ((opt = getopt(argc, argv, "Bbcfgi:l:o:sV")) != -1)
		switch (opt) {
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
		case 'l':
			lang = optarg;
			break;
		case 'o':
			processing_opt.push_back(optarg);
			break;
		case 's':
			symbolic_output = true;
			break;
		case 'V':
		    std::cout << "tokenizer " << version << std::endl;
		    exit(EXIT_SUCCESS);
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				"  [-cgs | -B | -b] [-fV] [-i file] [-l lang] [-o opt] [file ...]" << std::endl;
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
