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
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_using, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'(', ct.get_token());

		CSharpTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_while, ct2.get_token());

		CSharpTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct3.get_token());
	}

	void testIdentifier() {
		CSharpTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
	}

	void testCharacterToken() {
		CSharpTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
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
		CSharpTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::AND_EQUAL, ct.get_token());
	}

	void testBOOLEAN_AND() {
		CSharpTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_AND, ct.get_token());
	}

	void testBOOLEAN_OR() {
		CSharpTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_OR, ct.get_token());
	}

	void testDIV_EQUAL() {
		CSharpTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::DIV_EQUAL, ct.get_token());
	}

	void testEQUAL() {
		CSharpTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL((int)Token::EQUAL, ct.get_token());
	}

	void testGREATER_EQUAL() {
		CSharpTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::GREATER_EQUAL, ct.get_token());
	}

	void testLESS_EQUAL() {
		CSharpTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL((int)Token::LESS_EQUAL, ct.get_token());
	}

	void testLSHIFT() {
		CSharpTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT, ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		CSharpTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT_EQUAL, ct.get_token());
	}

	void testMINUS_EQUAL() {
		CSharpTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_EQUAL, ct.get_token());
	}

	void testMINUS_MINUS() {
		CSharpTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_MINUS, ct.get_token());
	}

	void testMOD_EQUAL() {
		CSharpTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MOD_EQUAL, ct.get_token());
	}

	void testNOT_EQUAL() {
		CSharpTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_EQUAL, ct.get_token());
	}

	void testOR_EQUAL() {
		CSharpTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL((int)Token::OR_EQUAL, ct.get_token());
	}

	void testPLUS_EQUAL() {
		CSharpTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_EQUAL, ct.get_token());
	}

	void testPLUS_PLUS() {
		CSharpTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_PLUS, ct.get_token());
	}

	void testRSHIFT() {
		CSharpTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testRSHIFT_EQUAL() {
		CSharpTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testTIMES_EQUAL() {
		CSharpTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::TIMES_EQUAL, ct.get_token());
	}

	void testXOR_EQUAL() {
		CSharpTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL((int)Token::XOR_EQUAL, ct.get_token());
	}

	void testNULL_CONDITIONAL() {
		CSharpTokenizer ct("?.b");
		CPPUNIT_ASSERT_EQUAL((int)Token::NULL_CONDITIONAL, ct.get_token());
	}

	void testNULL_COALESCE() {
		CSharpTokenizer ct("??a");
		CPPUNIT_ASSERT_EQUAL((int)Token::NULL_COALESCE, ct.get_token());
	}

	void testMEMBER_PTR() {
		CSharpTokenizer ct("->a");
		CPPUNIT_ASSERT_EQUAL((int)Token::MEMBER_PTR, ct.get_token());
	}

	void testLAMBDA() {
		CSharpTokenizer ct("=>x");
		CPPUNIT_ASSERT_EQUAL((int)Token::LAMBDA, ct.get_token());
	}

	void testBLOCK_COMMENT() {
		CSharpTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::BLOCK_COMMENT, ct.get_token());
	}

	void testDOC_COMMENT() {
		CSharpTokenizer ct("/// hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::DOC_COMMENT, ct.get_token());
		CSharpTokenizer ct2("\t/// hi\n\t/// there\n =>");
		CPPUNIT_ASSERT_EQUAL((int)Token::DOC_COMMENT, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::LAMBDA, ct2.get_token());
	}

	void testLINE_COMMENT() {
		CSharpTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct.get_token());
	}

	void testOptions() {
		CSharpTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(CSharpTokenizer::PT_METHOD, ct.get_processing_type());

		CSharpTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(CSharpTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

	void testSameScope() {
		CSharpTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
	}

	void testDifferentScope() {
		CSharpTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'}', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER + 1, ct.get_token());
	}

	void testCppKeyword() {
		CSharpTokenizer ct("# include ");
		CPPUNIT_ASSERT_EQUAL((int)'#', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_include, ct.get_token());

		CSharpTokenizer ct2("#region ! double foo2 bar2\nint include \n # define elif\n # endregion");
		CPPUNIT_ASSERT_EQUAL((int)'#', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_region, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_int, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'#', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_define, ct2.get_token());
		CPPUNIT_ASSERT(TokenId::is_identifier(ct2.get_token()));
		CPPUNIT_ASSERT_EQUAL((int)'#', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_endregion, ct2.get_token());
	}
};
#endif /*  CSHARPTOKENIZERTEST_H */
