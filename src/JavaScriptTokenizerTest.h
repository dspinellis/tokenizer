#ifndef JAVASCRIPTTOKENIZERTEST_H
#define JAVASCRIPTTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "JavaScriptTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class JavaScriptTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(JavaScriptTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testRAISE);
	CPPUNIT_TEST(testLAMBDA);
	CPPUNIT_TEST(testRAISE_EQUAL);
	CPPUNIT_TEST(testEQUAL_STRICT);
	CPPUNIT_TEST(testNOT_EQUAL_STRICT);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testBOOLEAN_AND_EQUAL);
	CPPUNIT_TEST(testBOOLEAN_OR_EQUAL);
	CPPUNIT_TEST(testNULL_COALESCING);
	CPPUNIT_TEST(testNULL_COALESCING_EQUAL);
	CPPUNIT_TEST(testNULL_CONDITIONAL);
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
	CPPUNIT_TEST(testRSHIFT_LOGICAL);
	CPPUNIT_TEST(testRSHIFT_ARITHMETIC_EQUAL);
	CPPUNIT_TEST(testRSHIFT_LOGICAL_EQUAL);
	CPPUNIT_TEST(testTIMES_EQUAL);
	CPPUNIT_TEST(testXOR_EQUAL);
	CPPUNIT_TEST(testBLOCK_COMMENT);
	CPPUNIT_TEST(testDOC_COMMENT);
	CPPUNIT_TEST(testLINE_COMMENT);
	CPPUNIT_TEST(testOptions);
	CPPUNIT_TEST(testSameScope);
	CPPUNIT_TEST(testDifferentScope);
	CPPUNIT_TEST(testSimpleString);
	CPPUNIT_TEST(testEmptyString);
	CPPUNIT_TEST(testSimpleTemplateString);
	CPPUNIT_TEST(testNestedTemplateString);
	CPPUNIT_TEST(testTemplateNestedTemplateString);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		JavaScriptTokenizer ct("await (");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_await), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct.get_token());

		JavaScriptTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_while), ct2.get_token());

		JavaScriptTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct3.get_token());
	}

	void testIdentifier() {
		JavaScriptTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testCharacterToken() {
		JavaScriptTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#");
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

	void testRAISE() {
		JavaScriptTokenizer ct("**3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RAISE), ct.get_token());
	}

	void testLAMBDA() {
		JavaScriptTokenizer ct("=>f");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LAMBDA), ct.get_token());
	}

	void testRAISE_EQUAL() {
		JavaScriptTokenizer ct("**=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RAISE_EQUAL), ct.get_token());
	}

	void testEQUAL_STRICT() {
		JavaScriptTokenizer ct("===3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::EQUAL_STRICT), ct.get_token());
	}

	void testNOT_EQUAL_STRICT() {
		JavaScriptTokenizer ct("!===3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL_STRICT), ct.get_token());
	}

	void testAND_EQUAL() {
		JavaScriptTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_EQUAL), ct.get_token());
	}

	void testBOOLEAN_AND_EQUAL() {
		JavaScriptTokenizer ct("&&=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_AND_EQUAL), ct.get_token());
	}

	void testBOOLEAN_OR_EQUAL() {
		JavaScriptTokenizer ct("||=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_OR_EQUAL), ct.get_token());
	}

	void testNULL_COALESCING() {
		JavaScriptTokenizer ct("??a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NULL_COALESCING), ct.get_token());
	}

	void testNULL_COALESCING_EQUAL() {
		JavaScriptTokenizer ct("?" "?=a"); // Avoid trigraph
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NULL_COALESCING_EQUAL), ct.get_token());
	}

	void testNULL_CONDITIONAL() {
		JavaScriptTokenizer ct("?.a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NULL_CONDITIONAL), ct.get_token());
	}

	void testBOOLEAN_AND() {
		JavaScriptTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_AND), ct.get_token());
	}

	void testBOOLEAN_OR() {
		JavaScriptTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_OR), ct.get_token());
	}

	void testDIV_EQUAL() {
		JavaScriptTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DIV_EQUAL), ct.get_token());
	}

	void testELIPSIS() {
		JavaScriptTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::ELIPSIS), ct.get_token());
	}

	void testEQUAL() {
		JavaScriptTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::EQUAL), ct.get_token());
	}

	void testGREATER_EQUAL() {
		JavaScriptTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::GREATER_EQUAL), ct.get_token());
	}

	void testLESS_EQUAL() {
		JavaScriptTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LESS_EQUAL), ct.get_token());
	}

	void testLSHIFT() {
		JavaScriptTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT), ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		JavaScriptTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT_EQUAL), ct.get_token());
	}

	void testMINUS_EQUAL() {
		JavaScriptTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_EQUAL), ct.get_token());
	}

	void testMINUS_MINUS() {
		JavaScriptTokenizer ct("--a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_MINUS), ct.get_token());
	}

	void testMOD_EQUAL() {
		JavaScriptTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MOD_EQUAL), ct.get_token());
	}

	void testNOT_EQUAL() {
		JavaScriptTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL), ct.get_token());
	}

	void testOR_EQUAL() {
		JavaScriptTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::OR_EQUAL), ct.get_token());
	}

	void testPLUS_EQUAL() {
		JavaScriptTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_EQUAL), ct.get_token());
	}

	void testPLUS_PLUS() {
		JavaScriptTokenizer ct("++a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_PLUS), ct.get_token());
	}

	void testRSHIFT_ARITHMETIC() {
		JavaScriptTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_ARITHMETIC), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testRSHIFT_LOGICAL() {
		JavaScriptTokenizer ct(">>>2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_LOGICAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testRSHIFT_ARITHMETIC_EQUAL() {
		JavaScriptTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_ARITHMETIC_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testRSHIFT_LOGICAL_EQUAL() {
		JavaScriptTokenizer ct(">>>=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_LOGICAL_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testTIMES_EQUAL() {
		JavaScriptTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::TIMES_EQUAL), ct.get_token());
	}

	void testXOR_EQUAL() {
		JavaScriptTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::XOR_EQUAL), ct.get_token());
	}

	void testBLOCK_COMMENT() {
		JavaScriptTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());

		JavaScriptTokenizer ct2("/**/");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct2.get_token());
	}

	void testDOC_COMMENT() {
		JavaScriptTokenizer ct("/** hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_DOC_COMMENT), ct.get_token());
	}

	void testLINE_COMMENT() {
		JavaScriptTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
	}

	void testOptions() {
		JavaScriptTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(JavaScriptTokenizer::PT_METHOD, ct.get_processing_type());

		JavaScriptTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(JavaScriptTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

	void testSameScope() {
		JavaScriptTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testDifferentScope() {
		JavaScriptTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('}'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER + 1), ct.get_token());
	}

	void testSimpleString() {
		JavaScriptTokenizer ct("a'string' in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct.get_token());

		JavaScriptTokenizer ct2("a\"string\" in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct2.get_token());

		JavaScriptTokenizer ct3("a`string` in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct3.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct3.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct3.get_token());
	}

	void testEmptyString() {
		JavaScriptTokenizer ct("a'' in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct.get_token());

		JavaScriptTokenizer ct2("a\"\" in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct2.get_token());
	}

	void testSimpleTemplateString() {
		JavaScriptTokenizer ct("a`string ${42}` in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct.get_token());
	}

	void testNestedTemplateString() {
		JavaScriptTokenizer ct("a`string ${{42}}` in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct.get_token());
	}

	void testTemplateNestedTemplateString() {
		JavaScriptTokenizer ct("a`string ${{`42`} `5 + ${8}`}` in");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_in), ct.get_token());
	}
};
#endif /*  JAVASCRIPTTOKENIZERTEST_H */
