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

#include <stack>

#include "NestedClassState.h"

/** Called when a class token is encountered */
void NestedClassState::saw_class()
{
	state.push(SAW_CLASS);
}

/** Called when a class(-like) token possibly encountered can
 * no longer be used to define a class
 */
void NestedClassState::unsaw_class()
{
	if (state.top() == SAW_CLASS)
		state.pop();
}

/** Called when an opening brace is encountered */
void NestedClassState::saw_open_brace()
{
	switch (state.top()) {
	case OUTER:
		// C++ functions
		state.push(IN_METHOD);
		break;
	case SAW_CLASS:
		state.top() = IN_CLASS;
		break;
	case IN_CLASS:
	case IN_METHOD:
		state.push(IN_METHOD);
		break;
	}
}

/** Called when an closing brace is encountered */
void NestedClassState::saw_close_brace()
{
	switch (state.top()) {
	case OUTER:
	case SAW_CLASS:
		/* Should not happen; ignore */
		break;
	case IN_CLASS:
	case IN_METHOD:
		state.pop();
		break;
	}
}
