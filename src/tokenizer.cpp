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
#include <vector>

#include "errno.h"
#include "unistd.h"

#include "SymbolTable.h"
#include "CTokenizer.h"
#include "CppTokenizer.h"
#include "CSharpTokenizer.h"
#include "JavaTokenizer.h"
#include "PHPTokenizer.h"
#include "PythonTokenizer.h"

static bool symbolic_output = false;
static bool compress_ids = false;
static enum output_type {
	ot_tokens,	// Numeric or symbolic tokens
	ot_break, 	// Original tokens broken into lines
	ot_type_break,	// As above, tokens preceded by their type
} output_type = ot_tokens;

// Process and print the metrics of stdin
static void
process_file(const std::string lang, const std::vector<std::string> opt,
		std::string filename)
{
	CharSource cs;
	TokenizerBase *t;

	if (lang == "" || lang == "Java")
		t = new JavaTokenizer(cs, filename, opt);
	else if (lang == "C")
		t = new CTokenizer(cs, filename, opt);
	else if (lang == "CSharp" || lang == "C#")
		t = new CSharpTokenizer(cs, filename, opt);
	else if (lang == "C++")
		t = new CppTokenizer(cs, filename, opt);
	else if (lang == "PHP")
		t = new PHPTokenizer(cs, filename, opt);
	else if (lang == "Python")
		t = new PythonTokenizer(cs, filename, opt);
	else {
		std::cerr << "Unknown language specified." << std::endl;
		std::cerr << "The following languages are supported:" << std::endl;
		std::cerr << "\tC" << std::endl;
		std::cerr << "\tCSharp (or C#)" << std::endl;
		std::cerr << "\tC++" << std::endl;
		std::cerr << "\tJava" << std::endl;
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

/* Calculate and print C metrics for the standard input */
int
main(int argc, char * const argv[])
{
	std::ifstream in;
	int opt;
	std::string lang = "";
	std::vector<std::string> processing_opt;

	while ((opt = getopt(argc, argv, "Bbcgl:o:st")) != -1)
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
		case 'g':
			SymbolTable::disable_scoping();
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
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				" [-l lang] [-o opt] [-cgs | -B | -b] [file ...]" << std::endl;
			exit(EXIT_FAILURE);
		}

	if (!argv[optind]) {
		process_file(lang, processing_opt, "-");
		exit(EXIT_SUCCESS);
	}

	// Read from file, if specified
	while (argv[optind]) {
		in.open(argv[optind], std::ifstream::in);
		if (!in.good()) {
			std::cerr << "Unable to open " << argv[optind] <<
				": " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cin.rdbuf(in.rdbuf());
		process_file(lang, processing_opt, argv[optind]);
		in.close();
		optind++;
	}

	exit(EXIT_SUCCESS);
}
