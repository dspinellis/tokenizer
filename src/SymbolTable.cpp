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

#include <map>
#include <list>
#include <string>

#include "SymbolTable.h"

// Return a symbol's value, adding it if needed
int
SymbolTable::value(std::string symbol)
{
	Table::value_type &current_map = table.front();
	Map::iterator lb = current_map.lower_bound(symbol);

	// Look at current scope, keeping a hint for later insertion
	if (lb != current_map.end() && lb->first == symbol)
		return lb->second;

	// Search from the second scope onward
	auto i = table.begin();
	for (i++; i != table.end(); i++) {
		auto entry = i->find(symbol);
		if (entry != i->end())
			return entry->second;
	}

	// Not found; insert it the the current scope
	int val = next_symbol_value++;
	current_map.insert(lb, Map::value_type(symbol, val));
	return val;
}

void
SymbolTable::enter_scope()
{
	table.push_front(Table::value_type());
}

void
SymbolTable::exit_scope()
{
	if (table.size() > 1)
		table.pop_front();
}
