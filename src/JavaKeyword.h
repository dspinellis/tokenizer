/*-
 * Copyright 2014-2018 Diomidis Spinellis
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

#ifndef JAVAKEYWORD_H
#define JAVAKEYWORD_H

#include <map>
#include <string>

/** Classify identifiers into Java keywords */
class JavaKeyword {
public:
	enum IdentifierType {
		IDENTIFIER = 256,	// Plain identifier (not a keyword)
		ABSTRACT,
		ASSERT,
		BOOLEAN,
		BREAK,
		BYTE,
		CASE,
		CATCH,
		CHAR,
		CLASS,
		CONST,
		CONTINUE,
		DEFAULT,
		DO,
		DOUBLE,
		ELSE,
		ENUM,
		EXTENDS,
		FINAL,
		FINALLY,
		FLOAT,
		FOR,
		GOTO,
		IF,
		IMPLEMENTS,
		IMPORT,
		INSTANCEOF,
		INT,
		INTERFACE,
		LONG,
		NATIVE,
		NEW,
		PACKAGE,
		PRIVATE,
		PROTECTED,
		PUBLIC,
		RETURN,
		SHORT,
		STATIC,
		STRICTFP,
		SUPER,
		SWITCH,
		SYNCHRONIZED,
		THIS,
		THROW,
		THROWS,
		TRANSIENT,
		TRY,
		VOID,
		VOLATILE,
		WHILE,
		LAST,
	};
private:
	// Keyword map
	typedef std::map <std::string, enum IdentifierType> KeywordMap;
	KeywordMap km;
public:
	JavaKeyword() {
		km["abstract"] = ABSTRACT;
		km["assert"] = ASSERT;
		km["boolean"] = BOOLEAN;
		km["break"] = BREAK;
		km["byte"] = BYTE;
		km["case"] = CASE;
		km["catch"] = CATCH;
		km["char"] = CHAR;
		km["class"] = CLASS;
		km["const"] = CONST;
		km["continue"] = CONTINUE;
		km["default"] = DEFAULT;
		km["do"] = DO;
		km["double"] = DOUBLE;
		km["else"] = ELSE;
		km["enum"] = ENUM;
		km["extends"] = EXTENDS;
		km["final"] = FINAL;
		km["finally"] = FINALLY;
		km["float"] = FLOAT;
		km["for"] = FOR;
		km["goto"] = GOTO;
		km["if"] = IF;
		km["implements"] = IMPLEMENTS;
		km["import"] = IMPORT;
		km["instanceof"] = INSTANCEOF;
		km["int"] = INT;
		km["interface"] = INTERFACE;
		km["long"] = LONG;
		km["native"] = NATIVE;
		km["new"] = NEW;
		km["package"] = PACKAGE;
		km["private"] = PRIVATE;
		km["protected"] = PROTECTED;
		km["public"] = PUBLIC;
		km["return"] = RETURN;
		km["short"] = SHORT;
		km["static"] = STATIC;
		km["strictfp"] = STRICTFP;
		km["super"] = SUPER;
		km["switch"] = SWITCH;
		km["synchronized"] = SYNCHRONIZED;
		km["this"] = THIS;
		km["throw"] = THROW;
		km["throws"] = THROWS;
		km["transient"] = TRANSIENT;
		km["try"] = TRY;
		km["void"] = VOID;
		km["volatile"] = VOLATILE;
		km["while"] = WHILE;
	}

	enum IdentifierType identifier_type(const std::string &s) {
		KeywordMap::const_iterator f = km.find(s);
		if (f == km.end())
			return IDENTIFIER;
		else
			return f->second;
	}
};
#endif /* JAVAKEYWORD_H */
