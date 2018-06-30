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
	void testBranch() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("if"), Keyword::IF);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("elif"), Keyword::ELIF);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("ifdef"), Keyword::IFDEF);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("for"), Keyword::FOR);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("while"), Keyword::WHILE);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("case"), Keyword::CASE);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("default"), Keyword::DEFAULT);
	}

	void testGoto() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("goto"), Keyword::GOTO);
	}

	void testRegister() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("register"), Keyword::REGISTER);
	}

	void testInclude() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("include"), Keyword::INCLUDE);
	}

	void testTypedef() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("typedef"), Keyword::TYPEDEF);
	}

	void testId() {
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("returning"), Keyword::IDENTIFIER);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("id"), Keyword::IDENTIFIER);
		CPPUNIT_ASSERT_EQUAL(ck.identifier_type("xyzzy"), Keyword::IDENTIFIER);
	}
};
#endif /*  CKEYWORDTEST_H */
