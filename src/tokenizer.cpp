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
#include "JavaTokenizer.h"

// Process and print the metrics of stdin
static void
process_file(const std::string lang, const std::vector<std::string> opt,
		std::string filename)
{
	CharSource cs;

	if (lang == "" || lang == "Java") {
		JavaTokenizer t(cs, opt);
		t.tokenize();
	} else if (lang == "C") {
		CTokenizer t(cs, opt);
		t.tokenize();
	} else {
		std::cerr << "Unknown language specified." << std::endl;
		std::cerr << "The following languages are supported:" << std::endl;
		std::cerr << "\tC" << std::endl;
		std::cerr << "\tJava" << std::endl;
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

	while ((opt = getopt(argc, argv, "l:o:")) != -1)
		switch (opt) {
		case 'l':
			lang = optarg;
			break;
		case 'o':
			processing_opt.push_back(optarg);
			break;
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				" [-l lang] [-o opt] [file ...]" << std::endl;
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
