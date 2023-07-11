/*-
 * Copyright 2015 Diomidis Spinellis
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

#include <cppunit/ui/text/TestRunner.h>

#include "BolStateTest.h"
#include "CharSourceTest.h"
#include "CKeywordTest.h"
#include "CTokenizerTest.h"
#include "CppTokenizerTest.h"
#include "CSharpTokenizerTest.h"
#include "JavaTokenizerTest.h"
#include "JavaScriptTokenizerTest.h"
#include "PHPTokenizerTest.h"
#include "PythonTokenizerTest.h"
#include "TokenizerBaseTest.h"
#include "SymbolTableTest.h"
#include "NestedClassStateTest.h"

int
main(int argc, char *argv[])
{
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(BolStateTest::suite());
	runner.addTest(CharSourceTest::suite());
	runner.addTest(CKeywordTest::suite());
	runner.addTest(TokenizerBaseTest::suite());
	runner.addTest(CTokenizerTest::suite());
	runner.addTest(CppTokenizerTest::suite());
	runner.addTest(CSharpTokenizerTest::suite());
	runner.addTest(JavaTokenizerTest::suite());
	runner.addTest(PythonTokenizerTest::suite());
	runner.addTest(PHPTokenizerTest::suite());
	runner.addTest(JavaScriptTokenizerTest::suite());
	runner.addTest(SymbolTableTest::suite());
	runner.addTest(NestedClassStateTest::suite());

	runner.run();
	return 0;
}
