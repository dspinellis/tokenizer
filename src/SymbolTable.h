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

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include <list>

#include "TokenId.h"

/**
 * Store and retrieve mappings from symbols to integers in a scoped
 * symbol table. Identifiers are entered in a map in the current scope
 * and retrieved from the first scope nearest to the current one in which
 * they appear.
 */
class SymbolTable {
	int next_symbol_value;
	typedef std::map<std::string, int> Map;	// Map at a single scope
	typedef std::list <Map> Table;		// Table of all scopes
	Table table;
public:
	/** Construct an empty symbol table */
	SymbolTable() : next_symbol_value(TokenId::IDENTIFIER), table(1) {}

	/** Return a symbol's value, adding it if needed */
	int value(std::string symbol);
	void enter_scope();
	void exit_scope();
	/** Return the current scope depth, with 0 being the outer scope */
	int scope_depth() const { return table.size() - 1; }
};
#endif /* SYMBOLTABLE_H */
