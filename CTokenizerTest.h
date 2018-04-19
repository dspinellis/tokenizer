#ifndef CTOKENIZERTEST_H
#define CTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CTokenizer.h"
#include "CToken.h"
#include "CKeyword.h"

class CTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(CTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testARROW);
	CPPUNIT_TEST(testBOOLEAN_AND);
	CPPUNIT_TEST(testBOOLEAN_OR);
	CPPUNIT_TEST(testDIV_EQUAL);
	CPPUNIT_TEST(testDOUBLE_COLON);
	CPPUNIT_TEST(testELIPSIS);
	CPPUNIT_TEST(testEQUAL);
	CPPUNIT_TEST(testGREATER_EQUAL);
	CPPUNIT_TEST(testLESS_EQUAL);
	CPPUNIT_TEST(testLSHIFT);
	CPPUNIT_TEST(testLSHIFT_EQUAL);
	CPPUNIT_TEST(testMINUS_EQUAL);
	CPPUNIT_TEST(testMINUS_MINUS);
	CPPUNIT_TEST(testMOD_EQUAL);
	CPPUNIT_TEST(testNOT_EQUAL);
	CPPUNIT_TEST(testOR_EQUAL);
	CPPUNIT_TEST(testPLUS_EQUAL);
	CPPUNIT_TEST(testPLUS_PLUS);
	CPPUNIT_TEST(testRSHIFT);
	CPPUNIT_TEST(testRSHIFT_EQUAL);
	CPPUNIT_TEST(testTIMES_EQUAL);
	CPPUNIT_TEST(testXOR_EQUAL);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		CTokenizer ct("do ");
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::DO, ct.get_token());

		CTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::WHILE, ct2.get_token());

		CTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::IF, ct3.get_token());
	}

	void testIdentifier() {
		CTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::IDENTIFIER, ct.get_token());
	}

	void testAND_EQUAL() {
		CTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL((int)CToken::AND_EQUAL, ct.get_token());
	}

	void testARROW() {
		CTokenizer ct("->a");
		CPPUNIT_ASSERT_EQUAL((int)CToken::ARROW, ct.get_token());
	}

	void testBOOLEAN_AND() {
		CTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL((int)CToken::BOOLEAN_AND, ct.get_token());
	}

	void testBOOLEAN_OR() {
		CTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL((int)CToken::BOOLEAN_OR, ct.get_token());
	}

	void testDIV_EQUAL() {
		CTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL((int)CToken::DIV_EQUAL, ct.get_token());
	}

	void testDOUBLE_COLON() {
		CTokenizer ct("::");
		CPPUNIT_ASSERT_EQUAL((int)CToken::DOUBLE_COLON, ct.get_token());
	}

	void testELIPSIS() {
		CTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL((int)CToken::ELIPSIS, ct.get_token());
	}

	void testEQUAL() {
		CTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL((int)CToken::EQUAL, ct.get_token());
	}

	void testGREATER_EQUAL() {
		CTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL((int)CToken::GREATER_EQUAL, ct.get_token());
	}

	void testIDENTIFIER() {
		CTokenizer ct("foo");
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::IDENTIFIER, ct.get_token());
	}

	void testLESS_EQUAL() {
		CTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL((int)CToken::LESS_EQUAL, ct.get_token());
	}

	void testLSHIFT() {
		CTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL((int)CToken::LSHIFT, ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		CTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL((int)CToken::LSHIFT_EQUAL, ct.get_token());
	}

	void testMINUS_EQUAL() {
		CTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL((int)CToken::MINUS_EQUAL, ct.get_token());
	}

	void testMINUS_MINUS() {
		CTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL((int)CToken::MINUS_MINUS, ct.get_token());
	}

	void testMOD_EQUAL() {
		CTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL((int)CToken::MOD_EQUAL, ct.get_token());
	}

	void testNOT_EQUAL() {
		CTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL((int)CToken::NOT_EQUAL, ct.get_token());
	}

	void testOR_EQUAL() {
		CTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL((int)CToken::OR_EQUAL, ct.get_token());
	}

	void testPLUS_EQUAL() {
		CTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL((int)CToken::PLUS_EQUAL, ct.get_token());
	}

	void testPLUS_PLUS() {
		CTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL((int)CToken::PLUS_PLUS, ct.get_token());
	}

	void testRSHIFT() {
		CTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL((int)CToken::RSHIFT, ct.get_token());
	}

	void testRSHIFT_EQUAL() {
		CTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL((int)CToken::RSHIFT_EQUAL, ct.get_token());
	}

	void testTIMES_EQUAL() {
		CTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL((int)CToken::TIMES_EQUAL, ct.get_token());
	}

	void testXOR_EQUAL() {
		CTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL((int)CToken::XOR_EQUAL, ct.get_token());
	}
};
#endif /*  CTOKENIZERTEST_H */
