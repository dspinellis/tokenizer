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

#ifndef CKEYWORDTEST_H
#define CKEYWORDTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "Keyword.h"

class CKeywordTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CKeywordTest);
	CPPUNIT_TEST(testBranch);
	CPPUNIT_TEST(testGoto);
	CPPUNIT_TEST(testRegister);
	CPPUNIT_TEST(testTypedef);
	CPPUNIT_TEST(testId);
	CPPUNIT_TEST(testInclude);
	CPPUNIT_TEST_SUITE_END();
	Keyword ck;
public:
	CKeywordTest() : ck(Keyword::L_C) { CppUnit::TestFixture(); }
	void testBranch() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("if"), Keyword::K_if);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("elif"), Keyword::K_elif);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("ifdef"), Keyword::K_ifdef);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("for"), Keyword::K_for);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("while"), Keyword::K_while);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("case"), Keyword::K_case);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("default"), Keyword::K_default);
	}

	void testGoto() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("goto"), Keyword::K_goto);
	}

	void testRegister() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("register"), Keyword::K_register);
	}

	void testInclude() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("include"), Keyword::K_include);
	}

	void testTypedef() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("typedef"), Keyword::K_typedef);
	}

	void testId() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("returning"), Keyword::IDENTIFIER);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("id"), Keyword::IDENTIFIER);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("xyzzy"), Keyword::IDENTIFIER);
	}
};
#endif /*  CKEYWORDTEST_H */
