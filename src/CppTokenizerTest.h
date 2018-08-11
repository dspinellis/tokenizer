#ifndef CPPTOKENIZERTEST_H
#define CPPTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CppTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class CppTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(CppTokenizerTest);
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
	CPPUNIT_TEST(testRSHIFT);
	CPPUNIT_TEST(testRSHIFT_EQUAL);
	CPPUNIT_TEST(testTIMES_EQUAL);
	CPPUNIT_TEST(testXOR_EQUAL);
	CPPUNIT_TEST(testMEMBER_PTR_FROM_OBJECT_PTR);
	CPPUNIT_TEST(testMEMBER_FROM_OBJECT_PTR);
	CPPUNIT_TEST(testMEMBER_PTR_FROM_OBJECT);
	CPPUNIT_TEST(testSCOPE);
	CPPUNIT_TEST(testAND);
	CPPUNIT_TEST(testBITOR);
	CPPUNIT_TEST(testOR);
	CPPUNIT_TEST(testXOR);
	CPPUNIT_TEST(testCOMPL);
	CPPUNIT_TEST(testBITAND);
	CPPUNIT_TEST(testAND_EQ);
	CPPUNIT_TEST(testOR_EQ);
	CPPUNIT_TEST(testXOR_EQ);
	CPPUNIT_TEST(testNOT);
	CPPUNIT_TEST(testNOT_EQ);
	CPPUNIT_TEST(testLINE_COMMENT);
	CPPUNIT_TEST(testJAVADOC_COMMENT);
	CPPUNIT_TEST(testBLOCK_COMMENT);
	CPPUNIT_TEST(testOptions);
	CPPUNIT_TEST(testSameScope);
	CPPUNIT_TEST(testDifferentScope);
	CPPUNIT_TEST(testCppKeyword);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		CppTokenizer ct("dynamic_cast <>");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_dynamic_cast, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'<', ct.get_token());

		CppTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_while, ct2.get_token());

		CppTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct3.get_token());
	}

	void testIdentifier() {
		CppTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
	}

	void testCharacterToken() {
		CppTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
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
		CppTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::AND_EQUAL, ct.get_token());
	}

	void testBOOLEAN_AND() {
		CppTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_AND, ct.get_token());
	}

	void testBOOLEAN_OR() {
		CppTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_OR, ct.get_token());
	}

	void testDIV_EQUAL() {
		CppTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::DIV_EQUAL, ct.get_token());
	}

	void testELIPSIS() {
		CppTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL((int)Token::ELIPSIS, ct.get_token());
	}

	void testEQUAL() {
		CppTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL((int)Token::EQUAL, ct.get_token());
	}

	void testGREATER_EQUAL() {
		CppTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::GREATER_EQUAL, ct.get_token());
	}

	void testLESS_EQUAL() {
		CppTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL((int)Token::LESS_EQUAL, ct.get_token());
	}

	void testLSHIFT() {
		CppTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT, ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		CppTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT_EQUAL, ct.get_token());
	}

	void testMINUS_EQUAL() {
		CppTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_EQUAL, ct.get_token());
	}

	void testMINUS_MINUS() {
		CppTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL((int)Token::MINUS_MINUS, ct.get_token());
	}

	void testMOD_EQUAL() {
		CppTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL((int)Token::MOD_EQUAL, ct.get_token());
	}

	void testNOT_EQUAL() {
		CppTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_EQUAL, ct.get_token());
	}

	void testOR_EQUAL() {
		CppTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL((int)Token::OR_EQUAL, ct.get_token());
	}

	void testPLUS_EQUAL() {
		CppTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_EQUAL, ct.get_token());
	}

	void testPLUS_PLUS() {
		CppTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL((int)Token::PLUS_PLUS, ct.get_token());
	}

	void testRSHIFT() {
		CppTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}
	void testRSHIFT_EQUAL() {
		CppTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL((int)Token::RSHIFT_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)1502, ct.get_token());
	}

	void testTIMES_EQUAL() {
		CppTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL((int)Token::TIMES_EQUAL, ct.get_token());
	}

	void testMEMBER_PTR_FROM_OBJECT_PTR() {
		CppTokenizer ct("->*v");
		CPPUNIT_ASSERT_EQUAL((int)Token::MEMBER_PTR_FROM_OBJECT_PTR, ct.get_token());
	}

	void testMEMBER_FROM_OBJECT_PTR() {
		CppTokenizer ct("->a");
		CPPUNIT_ASSERT_EQUAL((int)Token::MEMBER_FROM_OBJECT_PTR, ct.get_token());
	}

	void testMEMBER_PTR_FROM_OBJECT() {
		CppTokenizer ct(".*x");
		CPPUNIT_ASSERT_EQUAL((int)Token::MEMBER_PTR_FROM_OBJECT, ct.get_token());
	}

	void testSCOPE() {
		CppTokenizer ct("::a");
		CPPUNIT_ASSERT_EQUAL((int)Token::SCOPE, ct.get_token());
	}

	void testAND() {
		CppTokenizer ct("&&x");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_AND, ct.get_token());
	}

	void testBITOR() {
		CppTokenizer ct("|x");
		CPPUNIT_ASSERT_EQUAL((int)'|', ct.get_token());
	}

	void testOR() {
		CppTokenizer ct("||x");
		CPPUNIT_ASSERT_EQUAL((int)Token::BOOLEAN_OR, ct.get_token());
	}

	void testXOR() {
		CppTokenizer ct("^x");
		CPPUNIT_ASSERT_EQUAL((int)'^', ct.get_token());
	}

	void testCOMPL() {
		CppTokenizer ct("~x");
		CPPUNIT_ASSERT_EQUAL((int)'~', ct.get_token());
	}

	void testBITAND() {
		CppTokenizer ct("&x");
		CPPUNIT_ASSERT_EQUAL((int)'&', ct.get_token());
	}

	void testAND_EQ() {
		CppTokenizer ct("&=x");
		CPPUNIT_ASSERT_EQUAL((int)Token::AND_EQUAL, ct.get_token());
	}

	void testOR_EQ() {
		CppTokenizer ct("|=x");
		CPPUNIT_ASSERT_EQUAL((int)Token::OR_EQUAL, ct.get_token());
	}

	void testXOR_EQ() {
		CppTokenizer ct("^=x");
		CPPUNIT_ASSERT_EQUAL((int)Token::XOR_EQUAL, ct.get_token());
	}

	void testNOT() {
		CppTokenizer ct("!x");
		CPPUNIT_ASSERT_EQUAL((int)'!', ct.get_token());
	}

	void testNOT_EQ() {
		CppTokenizer ct("!=x");
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_EQUAL, ct.get_token());
	}

	void testXOR_EQUAL() {
		CppTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL((int)Token::XOR_EQUAL, ct.get_token());
	}

	void testBLOCK_COMMENT() {
		CppTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::BLOCK_COMMENT, ct.get_token());
	}

	void testJAVADOC_COMMENT() {
		CppTokenizer ct("/** hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::JAVADOC_COMMENT, ct.get_token());
	}

	void testLINE_COMMENT() {
		CppTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct.get_token());
	}

	void testOptions() {
		CppTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(CppTokenizer::PT_METHOD, ct.get_processing_type());

		CppTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(CppTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

	void testSameScope() {
		CppTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
	}

	void testDifferentScope() {
		CppTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL((int)'{', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'}', ct.get_token());
		CPPUNIT_ASSERT_EQUAL(TokenId::IDENTIFIER + 1, ct.get_token());
	}

	void testCppKeyword() {
		CTokenizer ct("# include ");
		CPPUNIT_ASSERT_EQUAL((int)'#', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_include, ct.get_token());

		CTokenizer ct2("int include \n # define elif");
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_int, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'#', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_define, ct2.get_token());
		CPPUNIT_ASSERT(TokenId::is_identifier(ct2.get_token()));
	}

};
#endif /*  CPPTOKENIZERTEST_H */
