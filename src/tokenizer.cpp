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

#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

#include "errno.h"
#include "unistd.h"

#include "CTokenizer.h"
#include "CppTokenizer.h"
#include "CSharpTokenizer.h"
#include "JavaTokenizer.h"
#include "PHPTokenizer.h"
#include "PythonTokenizer.h"

// Process and print the metrics of stdin
static void
process_file(const std::string lang, const std::vector<std::string> opt,
		std::string filename, char processing_type)
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
		exit(1);
	}
	switch (processing_type) {
	case 'c':
		t->code_tokenize();
		break;
	case 'n':
		t->numeric_tokenize();
		break;
	case 's':
		t->symbolic_tokenize();
		break;
	case 't':
		t->type_tokenize();
		break;
	case 'T':
		t->type_code_tokenize();
		break;
	default:
		std::cerr << "Unknown processing type specified." << std::endl;
		std::cerr << "The following processing types are supported:" << std::endl;
		std::cerr << "\tc: output code; one token per line" << std::endl;
		std::cerr << "\tn: output numeric values" << std::endl;
		std::cerr << "\ts: output token symbols" << std::endl;
		std::cerr << "\tt: output token types" << std::endl;
		std::cerr << "\tT: output token types and code; one token per line" << std::endl;
		exit(1);
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
	char processing_type = 'n';

	while ((opt = getopt(argc, argv, "l:o:t:")) != -1)
		switch (opt) {
		case 'l':
			lang = optarg;
			break;
		case 'o':
			processing_opt.push_back(optarg);
			break;
		case 't':
			processing_type = *optarg;
			break;
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				" [-l lang] [-o opt] [-t type] [file ...]" << std::endl;
			exit(EXIT_FAILURE);
		}

	if (!argv[optind]) {
		process_file(lang, processing_opt, "-", processing_type);
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
		process_file(lang, processing_opt, argv[optind], processing_type);
		in.close();
		optind++;
	}

	exit(EXIT_SUCCESS);
}
