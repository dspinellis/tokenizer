#ifndef JAVATOKENIZERTEST_H
#define JAVATOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "JavaTokenizer.h"
#include "JavaToken.h"
#include "JavaKeyword.h"

class JavaTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(JavaTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testBOOLEAN_AND);
	CPPUNIT_TEST(testBOOLEAN_OR);
	CPPUNIT_TEST(testDIV_EQUAL);
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
	CPPUNIT_TEST(testRSHIFT_ARITHMETIC);
	CPPUNIT_TEST(testRSHIFT_ARITHMETIC_EQUAL);
	CPPUNIT_TEST(testRSHIFT_LOGICAL);
	CPPUNIT_TEST(testRSHIFT_LOGICAL_EQUAL);
	CPPUNIT_TEST(testTIMES_EQUAL);
	CPPUNIT_TEST(testXOR_EQUAL);
	CPPUNIT_TEST(testOptions);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		JavaTokenizer ct("instanceof (");
		CPPUNIT_ASSERT_EQUAL((int)JavaKeyword::INSTANCEOF, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'(', ct.get_token());

		JavaTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)JavaKeyword::WHILE, ct2.get_token());

		JavaTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)JavaKeyword::IF, ct3.get_token());
	}

	void testIdentifier() {
		JavaTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::IDENTIFIER, ct.get_token());
		JavaTokenizer ct2("Foo");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::CLASS_NAME, ct2.get_token());
	}

	void testCharacterToken() {
		JavaTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
		CPPUNIT_ASSERT_EQUAL((int)'+', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'-', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'*', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'/', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'=', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'<', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'>', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'%', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'(', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)')', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'[', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)']', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'}', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'^', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'|', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'&', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'~', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)',', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'.', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)';', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)':', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'!', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'#', ct.get_token());
	}

	void testAND_EQUAL() {
		JavaTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::AND_EQUAL, ct.get_token());
	}

	void testBOOLEAN_AND() {
		JavaTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::BOOLEAN_AND, ct.get_token());
	}

	void testBOOLEAN_OR() {
		JavaTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::BOOLEAN_OR, ct.get_token());
	}

	void testDIV_EQUAL() {
		JavaTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::DIV_EQUAL, ct.get_token());
	}

	void testELIPSIS() {
		JavaTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::ELIPSIS, ct.get_token());
	}

	void testEQUAL() {
		JavaTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::EQUAL, ct.get_token());
	}

	void testGREATER_EQUAL() {
		JavaTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::GREATER_EQUAL, ct.get_token());
	}

	void testLESS_EQUAL() {
		JavaTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::LESS_EQUAL, ct.get_token());
	}

	void testLSHIFT() {
		JavaTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::LSHIFT, ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		JavaTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::LSHIFT_EQUAL, ct.get_token());
	}

	void testMINUS_EQUAL() {
		JavaTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::MINUS_EQUAL, ct.get_token());
	}

	void testMINUS_MINUS() {
		JavaTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::MINUS_MINUS, ct.get_token());
	}

	void testMOD_EQUAL() {
		JavaTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::MOD_EQUAL, ct.get_token());
	}

	void testNOT_EQUAL() {
		JavaTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::NOT_EQUAL, ct.get_token());
	}

	void testOR_EQUAL() {
		JavaTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::OR_EQUAL, ct.get_token());
	}

	void testPLUS_EQUAL() {
		JavaTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::PLUS_EQUAL, ct.get_token());
	}

	void testPLUS_PLUS() {
		JavaTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::PLUS_PLUS, ct.get_token());
	}

	void testRSHIFT_ARITHMETIC() {
		JavaTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::RSHIFT_ARITHMETIC, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_LOGICAL() {
		JavaTokenizer ct(">>>2");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::RSHIFT_LOGICAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_ARITHMETIC_EQUAL() {
		JavaTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::RSHIFT_ARITHMETIC_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_LOGICAL_EQUAL() {
		JavaTokenizer ct(">>>=5");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::RSHIFT_LOGICAL_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testTIMES_EQUAL() {
		JavaTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::TIMES_EQUAL, ct.get_token());
	}

	void testXOR_EQUAL() {
		JavaTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL((int)JavaToken::XOR_EQUAL, ct.get_token());
	}

	void testOptions() {
		JavaTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(JavaTokenizer::PT_METHOD, ct.get_processing_type());

		JavaTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(JavaTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

};
#endif /*  JAVATOKENIZERTEST_H */
