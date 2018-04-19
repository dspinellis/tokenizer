#ifndef TOKENIZERBASETEST_H
#define TOKENIZERBASETEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CTokenizer.h"
#include "CToken.h"
#include "CKeyword.h"

/*
 * Use he C tokenizer to test functionality in the TokenizerBase
 * class.
 */
class TokenizerBaseTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(TokenizerBaseTest);
	CPPUNIT_TEST(testCharLiteral);
	CPPUNIT_TEST(testStringLiteral);
	CPPUNIT_TEST(testComment);
	CPPUNIT_TEST(testNumber);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCharLiteral() {
		CTokenizer ct("'a'");
		CPPUNIT_ASSERT_EQUAL((int)CToken::CHAR_LITERAL, ct.get_token());

		CTokenizer ct2("'\\\'a'+");
		CPPUNIT_ASSERT_EQUAL((int)CToken::CHAR_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'+', ct2.get_token());

		CTokenizer ct3("'\\001'");
		CPPUNIT_ASSERT_EQUAL((int)CToken::CHAR_LITERAL, ct3.get_token());

		CTokenizer ct4("'\\xfa'");
		CPPUNIT_ASSERT_EQUAL((int)CToken::CHAR_LITERAL, ct4.get_token());
		CTokenizer ct5("L'a'");
		CPPUNIT_ASSERT_EQUAL((int)CToken::CHAR_LITERAL, ct5.get_token());
	}

	void testStringLiteral() {
		CTokenizer ct("\"hello\"");
		CPPUNIT_ASSERT_EQUAL((int)CToken::STRING_LITERAL, ct.get_token());

		CTokenizer ct2("\"he\\\"llo\"+");
		CPPUNIT_ASSERT_EQUAL((int)CToken::STRING_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'+', ct2.get_token());

		CTokenizer ct3("L\"hello\"");
		CPPUNIT_ASSERT_EQUAL((int)CToken::STRING_LITERAL, ct3.get_token());
	}

	void testComment() {
		CTokenizer ct(" /* block comment */ +");
		CPPUNIT_ASSERT_EQUAL((int)'+', ct.get_token());

		CTokenizer ct2("// line comment\n +");
		CPPUNIT_ASSERT_EQUAL((int)'+', ct2.get_token());

		CTokenizer ct3("/* hi * / */\n+");
		CPPUNIT_ASSERT_EQUAL((int)'+', ct3.get_token());

		CTokenizer ct4("/* hi ***/\n+");
		CPPUNIT_ASSERT_EQUAL((int)'+', ct4.get_token());
	}

	void testNumber() {
		CTokenizer ct("0");
		CPPUNIT_ASSERT_EQUAL(1500, ct.get_token());

		CTokenizer ct1("1");
		CPPUNIT_ASSERT_EQUAL(1501, ct1.get_token());

		CTokenizer ct2("1.1");
		CPPUNIT_ASSERT_EQUAL(1502, ct2.get_token());

		CTokenizer ct3("10");
		CPPUNIT_ASSERT_EQUAL(1502, ct3.get_token());

		CTokenizer ct3a("100");
		CPPUNIT_ASSERT_EQUAL(1503, ct3a.get_token());

		CTokenizer ct4("0.1");
		CPPUNIT_ASSERT_EQUAL(1499, ct4.get_token());

		CTokenizer ct5("1.1e300");
		int v5 = ct5.get_token();
		CPPUNIT_ASSERT(v5 > 1500);
		CPPUNIT_ASSERT(v5 < 2000);

		CTokenizer ct6("1.1E-300f");
		int v6 = ct6.get_token();
		CPPUNIT_ASSERT(v6 < 1500);
		CPPUNIT_ASSERT(v5 > 1000);

		CTokenizer ct7("0.1f+");
		CPPUNIT_ASSERT_EQUAL(1499, ct7.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'+', ct7.get_token());

		CTokenizer ct8("0.1d+");
		CPPUNIT_ASSERT_EQUAL(1499, ct8.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'+', ct8.get_token());
	}
};
#endif /*  TOKENIZERBASETEST_H */
