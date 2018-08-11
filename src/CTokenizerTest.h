#ifndef CTOKENIZERTEST_H
#define CTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CTokenizer.h"
#include "Token.h"
#include "TokenId.h"
#include "Keyword.h"

class CTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(CTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testARROW);
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
	CPPUNIT_TEST(testRSHIFT);
	CPPUNIT_TEST(testRSHIFT_EQUAL);
	CPPUNIT_TEST(testTIMES_EQUAL);
	CPPUNIT_TEST(testXOR_EQUAL);
	CPPUNIT_TEST(testLINE_COMMENT);
	CPPUNIT_TEST(testDOC_COMMENT);
	CPPUNIT_TEST(testBLOCK_COMMENT);
	CPPUNIT_TEST(testSameScope);
	CPPUNIT_TEST(testDifferentScope);
	CPPUNIT_TEST(testCppKeyword);
	CPPUNIT_TEST(testPasteOperator);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		CTokenizer ct("do ");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_do, ct.get_token());

		CTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_while, ct2.get_token());

		CTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct3.get_token());
	}

	void testIdentifier() {
		CTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
	}

	void testCharacterToken() {
		CTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
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
		CTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::AND_EQUAL, ct.get_token());
	}

	void testARROW() {
		CTokenizer ct("->a");
		CPPUNIT_ASSERT_EQUAL((int)Token::ARROW, ct.get_token());
	}

	void testBOOLEAN_AND() {
		CTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_AND, ct.get_token());
	}

	void testBOOLEAN_OR() {
		CTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_OR, ct.get_token());
	}

	void testDIV_EQUAL() {
		CTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::DIV_EQUAL, ct.get_token());
	}

	void testELIPSIS() {
		CTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL((int)Token::ELIPSIS, ct.get_token());
	}

	void testEQUAL() {
		CTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL((int)Token::EQUAL, ct.get_token());
	}

	void testGREATER_EQUAL() {
		CTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::GREATER_EQUAL, ct.get_token());
	}

	void testIDENTIFIER() {
		CTokenizer ct("foo");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::IDENTIFIER, ct.get_token());
	}

	void testLESS_EQUAL() {
		CTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL((int)Token::LESS_EQUAL, ct.get_token());
	}

	void testLSHIFT() {
		CTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT, ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		CTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT_EQUAL, ct.get_token());
	}

	void testMINUS_EQUAL() {
		CTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_EQUAL, ct.get_token());
	}

	void testMINUS_MINUS() {
		CTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_MINUS, ct.get_token());
	}

	void testMOD_EQUAL() {
		CTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MOD_EQUAL, ct.get_token());
	}

	void testNOT_EQUAL() {
		CTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_EQUAL, ct.get_token());
	}

	void testOR_EQUAL() {
		CTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL((int)Token::OR_EQUAL, ct.get_token());
	}

	void testPLUS_EQUAL() {
		CTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_EQUAL, ct.get_token());
	}

	void testPLUS_PLUS() {
		CTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_PLUS, ct.get_token());
	}

	void testRSHIFT() {
		CTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT, ct.get_token());
	}

	void testRSHIFT_EQUAL() {
		CTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_EQUAL, ct.get_token());
	}

	void testTIMES_EQUAL() {
		CTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::TIMES_EQUAL, ct.get_token());
	}

	void testXOR_EQUAL() {
		CTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL((int)Token::XOR_EQUAL, ct.get_token());
	}

	void testBLOCK_COMMENT() {
		CTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::BLOCK_COMMENT, ct.get_token());
	}

	void testLINE_COMMENT() {
		CTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct.get_token());
	}

	void testDOC_COMMENT() {
		CTokenizer ct("/** hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::DOC_COMMENT, ct.get_token());
	}

	void testSameScope() {
		CTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
	}

	void testDifferentScope() {
		CTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'}', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER + 1, ct.get_token());
	}

	void testCppKeyword() {
		CTokenizer ct("# include ");
		CPPUNIT_ASSERT_EQUAL((int)'#', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_include, ct.get_token());

		CTokenizer ct2("int include \n # define elif // foo\n# define");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_int, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'#', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_define, ct2.get_token());
		CPPUNIT_ASSERT(TokenId::is_identifier(ct2.get_token()));
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'#', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_define, ct2.get_token());
	}

	void testPasteOperator() {
		CTokenizer ct("a ## b");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::TOKEN_PASTE, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER + 1, ct.get_token());
	}
};
#endif /*  CTOKENIZERTEST_H */
