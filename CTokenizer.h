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

#ifndef CTOKENIZER_H
#define CTOKENIZER_H

#include <iostream>

#include "CharSource.h"
#include "CKeyword.h"

/** Collect quality metrics from C-like source code */
class CTokenizer {
private:
	CharSource src;
	bool scan_cpp_directive;	// Keyword after a C preprocessor #
	bool scan_cpp_line;		// Line after a C preprocessor #
	bool process_character();
	bool continuation;		// True if a statement continuation line
	/** True for keywords that don't end with semicolon */
	bool saw_comment;		// True after a comment
	bool saw_cpp_directive;		// True after c preprocessor directive
	CKeyword ckeyword;
	/** Called at every encountered newline */
	void newline(bool in_non_code_block = false) {}
public:
	void tokenize();
	CTokenizer() : scan_cpp_directive(false), scan_cpp_line(false),
	saw_comment(false), saw_cpp_directive(false) {}
};
#endif /* CTOKENIZER_H */
