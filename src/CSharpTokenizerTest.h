#ifndef CSHARPTOKENIZERTEST_H
#define CSHARPTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CSharpTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class CSharpTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(CSharpTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testBOOLEAN_AND);
	CPPUNIT_TEST(testBOOLEAN_OR);
	CPPUNIT_TEST(testDIV_EQUAL);
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
	CPPUNIT_TEST(testNULL_CONDITIONAL);
	CPPUNIT_TEST(testNULL_COALESCE);
	CPPUNIT_TEST(testMEMBER_PTR);
	CPPUNIT_TEST(testLAMBDA);
	CPPUNIT_TEST(testLINE_COMMENT);
	CPPUNIT_TEST(testDOC_COMMENT);
	CPPUNIT_TEST(testBLOCK_COMMENT);
	CPPUNIT_TEST(testOptions);
	CPPUNIT_TEST(testSameScope);
	CPPUNIT_TEST(testDifferentScope);
	CPPUNIT_TEST(testCppKeyword);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		CSharpTokenizer ct("using (");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_using), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct.get_token());

		CSharpTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_while), ct2.get_token());

		CSharpTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct3.get_token());
	}

	void testIdentifier() {
		CSharpTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testCharacterToken() {
		CSharpTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
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

	void testAND_EQUAL() {
		CSharpTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_EQUAL), ct.get_token());
	}

	void testBOOLEAN_AND() {
		CSharpTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_AND), ct.get_token());
	}

	void testBOOLEAN_OR() {
		CSharpTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_OR), ct.get_token());
	}

	void testDIV_EQUAL() {
		CSharpTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DIV_EQUAL), ct.get_token());
	}

	void testEQUAL() {
		CSharpTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::EQUAL), ct.get_token());
	}

	void testGREATER_EQUAL() {
		CSharpTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::GREATER_EQUAL), ct.get_token());
	}

	void testLESS_EQUAL() {
		CSharpTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LESS_EQUAL), ct.get_token());
	}

	void testLSHIFT() {
		CSharpTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT), ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		CSharpTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT_EQUAL), ct.get_token());
	}

	void testMINUS_EQUAL() {
		CSharpTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_EQUAL), ct.get_token());
	}

	void testMINUS_MINUS() {
		CSharpTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_MINUS), ct.get_token());
	}

	void testMOD_EQUAL() {
		CSharpTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MOD_EQUAL), ct.get_token());
	}

	void testNOT_EQUAL() {
		CSharpTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL), ct.get_token());
	}

	void testOR_EQUAL() {
		CSharpTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::OR_EQUAL), ct.get_token());
	}

	void testPLUS_EQUAL() {
		CSharpTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_EQUAL), ct.get_token());
	}

	void testPLUS_PLUS() {
		CSharpTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_PLUS), ct.get_token());
	}

	void testRSHIFT() {
		CSharpTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testRSHIFT_EQUAL() {
		CSharpTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testTIMES_EQUAL() {
		CSharpTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::TIMES_EQUAL), ct.get_token());
	}

	void testXOR_EQUAL() {
		CSharpTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::XOR_EQUAL), ct.get_token());
	}

	void testNULL_CONDITIONAL() {
		CSharpTokenizer ct("?.b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NULL_CONDITIONAL), ct.get_token());
	}

	void testNULL_COALESCE() {
		CSharpTokenizer ct("??a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NULL_COALESCE), ct.get_token());
	}

	void testMEMBER_PTR() {
		CSharpTokenizer ct("->a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MEMBER_PTR), ct.get_token());
	}

	void testLAMBDA() {
		CSharpTokenizer ct("=>x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LAMBDA), ct.get_token());
	}

	void testBLOCK_COMMENT() {
		CSharpTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
	}

	void testDOC_COMMENT() {
		CSharpTokenizer ct("/// hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DOC_COMMENT), ct.get_token());
		CSharpTokenizer ct2("\t/// hi\n\t/// there\n =>");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DOC_COMMENT), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LAMBDA), ct2.get_token());
	}

	void testLINE_COMMENT() {
		CSharpTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
	}

	void testOptions() {
		CSharpTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(CSharpTokenizer::PT_METHOD, ct.get_processing_type());

		CSharpTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(CSharpTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

	void testSameScope() {
		CSharpTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testDifferentScope() {
		CSharpTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('}'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER + 1), ct.get_token());
	}

	void testCppKeyword() {
		CSharpTokenizer ct("# define ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('#'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_define), ct.get_token());

		CSharpTokenizer ct2("#region ! double foo2 bar2\nint include \n # define elif\n # endregion");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('#'), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_region), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_int), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('#'), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_define), ct2.get_token());
		CPPUNIT_ASSERT(TokenId::is_identifier(ct2.get_token()));
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('#'), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_endregion), ct2.get_token());
	}
};
#endif /*  CSHARPTOKENIZERTEST_H */
