#ifndef JAVATOKENIZERTEST_H
#define JAVATOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "JavaTokenizer.h"
#include "Token.h"
#include "Keyword.h"

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
	CPPUNIT_TEST(testLINE_COMMENT);
	CPPUNIT_TEST(testJAVADOC_COMMENT);
	CPPUNIT_TEST(testBLOCK_COMMENT);
	CPPUNIT_TEST(testOptions);
	CPPUNIT_TEST(testSameScope);
	CPPUNIT_TEST(testDifferentScope);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		JavaTokenizer ct("instanceof (");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_instanceof, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'(', ct.get_token());

		JavaTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_while, ct2.get_token());

		JavaTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct3.get_token());
	}

	void testIdentifier() {
		JavaTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
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
		CPPUNIT_ASSERT_EQUAL((int)Token::AND_EQUAL, ct.get_token());
	}

	void testBOOLEAN_AND() {
		JavaTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_AND, ct.get_token());
	}

	void testBOOLEAN_OR() {
		JavaTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_OR, ct.get_token());
	}

	void testDIV_EQUAL() {
		JavaTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::DIV_EQUAL, ct.get_token());
	}

	void testELIPSIS() {
		JavaTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL((int)Token::ELIPSIS, ct.get_token());
	}

	void testEQUAL() {
		JavaTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL((int)Token::EQUAL, ct.get_token());
	}

	void testGREATER_EQUAL() {
		JavaTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::GREATER_EQUAL, ct.get_token());
	}

	void testLESS_EQUAL() {
		JavaTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL((int)Token::LESS_EQUAL, ct.get_token());
	}

	void testLSHIFT() {
		JavaTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT, ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		JavaTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT_EQUAL, ct.get_token());
	}

	void testMINUS_EQUAL() {
		JavaTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_EQUAL, ct.get_token());
	}

	void testMINUS_MINUS() {
		JavaTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_MINUS, ct.get_token());
	}

	void testMOD_EQUAL() {
		JavaTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MOD_EQUAL, ct.get_token());
	}

	void testNOT_EQUAL() {
		JavaTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_EQUAL, ct.get_token());
	}

	void testOR_EQUAL() {
		JavaTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL((int)Token::OR_EQUAL, ct.get_token());
	}

	void testPLUS_EQUAL() {
		JavaTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_EQUAL, ct.get_token());
	}

	void testPLUS_PLUS() {
		JavaTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_PLUS, ct.get_token());
	}

	void testRSHIFT_ARITHMETIC() {
		JavaTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_ARITHMETIC, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_LOGICAL() {
		JavaTokenizer ct(">>>2");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_LOGICAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_ARITHMETIC_EQUAL() {
		JavaTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_ARITHMETIC_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_LOGICAL_EQUAL() {
		JavaTokenizer ct(">>>=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_LOGICAL_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testTIMES_EQUAL() {
		JavaTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::TIMES_EQUAL, ct.get_token());
	}

	void testXOR_EQUAL() {
		JavaTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL((int)Token::XOR_EQUAL, ct.get_token());
	}

	void testBLOCK_COMMENT() {
		JavaTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::BLOCK_COMMENT, ct.get_token());
	}

	void testJAVADOC_COMMENT() {
		JavaTokenizer ct("/** hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::JAVADOC_COMMENT, ct.get_token());
	}

	void testLINE_COMMENT() {
		JavaTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct.get_token());
	}

	void testOptions() {
		JavaTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(JavaTokenizer::PT_METHOD, ct.get_processing_type());

		JavaTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(JavaTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

	void testSameScope() {
		JavaTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
	}

	void testDifferentScope() {
		JavaTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'}', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER + 1, ct.get_token());
	}
};
#endif /*  JAVATOKENIZERTEST_H */
