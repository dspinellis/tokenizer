/*-
 * Copyright 2018 Diomidis Spinellis
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

#ifndef NESTEDCLASSSTATE_H
#define NESTEDCLASSSTATE_H

#include <stack>

/**
 * Implement a state machine that allows the program to know whether
 * it is processing a method or a class body.
 *
 * Currently there is no provision to deal with the "class" keyword
 * appearing instead of "typename" in C++ code.
 * Such a provision would probably have to count template brackets <>
 * in the class and outer states and ignore the word class in method
 * bodies.
 */
class NestedClassState {
	typedef enum state {
		OUTER,
		SAW_CLASS,
		IN_CLASS,
		IN_METHOD,
	} State;
	typedef std::stack <State> StateStack;	// Table of all scopes
	StateStack state;
public:
	NestedClassState() {
		state.push(OUTER);
	}

	/** Called when a class(-like) token is encountered */
	void saw_class();

	/** Called when a class(-like) token possibly encountered can no longer be used to define a class */
	void unsaw_class();


	/** Called when an opening brace is encountered */
	void saw_open_brace();

	/** Called when an closing brace is encountered */
	void saw_close_brace();

	/** Return true if processing a method or function body */
	bool in_method() const { return state.top() == IN_METHOD; }
};
#endif /* NESTEDCLASSSTATE_H */
