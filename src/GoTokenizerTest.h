#ifndef GOTOKENIZERTEST_H
#define GOTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "GoTokenizer.h"
#include "Token.h"
#include "TokenId.h"
#include "Keyword.h"

class GoTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(GoTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testCHANNEL_DIRECTION);
	CPPUNIT_TEST(testAND_NOT_EQUAL);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testVAR_ASSIGN);
	CPPUNIT_TEST(testAND_NOT);
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
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		GoTokenizer ct("do ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_do), ct.get_token());

		GoTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_while), ct2.get_token());

		GoTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct3.get_token());
	}

	void testIdentifier() {
		GoTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testCharacterToken() {
		GoTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('-'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('*'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('/'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('='), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('<'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('>'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('%'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('['), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(']'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('}'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('^'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('|'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('&'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('~'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(','), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('.'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(';'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(':'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('!'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('#'), ct.get_token());
	}

	void testCHANNEL_DIRECTION() {
		GoTokenizer ct("<-b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHANNEL_DIRECTION), ct.get_token());
	}

	void testAND_NOT_EQUAL() {
		GoTokenizer ct("&^=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_NOT_EQUAL), ct.get_token());
	}

	void testAND_EQUAL() {
		GoTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_EQUAL), ct.get_token());
	}

	void testVAR_ASSIGN() {
		GoTokenizer ct("a:=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::VAR_ASSIGN), ct.get_token());
	}

	void testAND_NOT() {
		GoTokenizer ct("&^=b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_NOT_EQUAL), ct.get_token());
	}

	void testBOOLEAN_AND() {
		GoTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_AND), ct.get_token());
	}

	void testBOOLEAN_OR() {
		GoTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_OR), ct.get_token());
	}

	void testDIV_EQUAL() {
		GoTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DIV_EQUAL), ct.get_token());
	}

	void testELIPSIS() {
		GoTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::ELIPSIS), ct.get_token());
	}

	void testEQUAL() {
		GoTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::EQUAL), ct.get_token());
	}

	void testGREATER_EQUAL() {
		GoTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::GREATER_EQUAL), ct.get_token());
	}

	void testIDENTIFIER() {
		GoTokenizer ct("foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::FIRST_IDENTIFIER), ct.get_token());
	}

	void testLESS_EQUAL() {
		GoTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LESS_EQUAL), ct.get_token());
	}

	void testLSHIFT() {
		GoTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT), ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		GoTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT_EQUAL), ct.get_token());
	}

	void testMINUS_EQUAL() {
		GoTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_EQUAL), ct.get_token());
	}

	void testMINUS_MINUS() {
		GoTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_MINUS), ct.get_token());
	}

	void testMOD_EQUAL() {
		GoTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MOD_EQUAL), ct.get_token());
	}

	void testNOT_EQUAL() {
		GoTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL), ct.get_token());
	}

	void testOR_EQUAL() {
		GoTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::OR_EQUAL), ct.get_token());
	}

	void testPLUS_EQUAL() {
		GoTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_EQUAL), ct.get_token());
	}

	void testPLUS_PLUS() {
		GoTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_PLUS), ct.get_token());
	}

	void testRSHIFT() {
		GoTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT), ct.get_token());
	}

	void testRSHIFT_EQUAL() {
		GoTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_EQUAL), ct.get_token());
	}

	void testTIMES_EQUAL() {
		GoTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::TIMES_EQUAL), ct.get_token());
	}

	void testXOR_EQUAL() {
		GoTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::XOR_EQUAL), ct.get_token());
	}

	void testBLOCK_COMMENT() {
		GoTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
	}

	void testLINE_COMMENT() {
		GoTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
	}

	void testDOC_COMMENT() {
		GoTokenizer ct("/** hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DOC_COMMENT), ct.get_token());
	}

	void testSameScope() {
		GoTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testDifferentScope() {
		GoTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('}'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER + 1), ct.get_token());
	}

};
#endif /*  GOTOKENIZERTEST_H */
