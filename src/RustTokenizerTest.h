#ifndef RUSTTOKENIZERTEST_H
#define RUSTTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "RustTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class RustTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(RustTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testAND_EQUAL);
	CPPUNIT_TEST(testBOOLEAN_AND);
	CPPUNIT_TEST(testBOOLEAN_OR);
	CPPUNIT_TEST(testDIV_EQUAL);
	CPPUNIT_TEST(testELIPSIS);
	CPPUNIT_TEST(testDOT_DOT);
	CPPUNIT_TEST(testEQUAL);
	CPPUNIT_TEST(testRIGHT_FAT_ARROW);
	CPPUNIT_TEST(testGREATER_EQUAL);
	CPPUNIT_TEST(testLESS_EQUAL);
	CPPUNIT_TEST(testLSHIFT);
	CPPUNIT_TEST(testLSHIFT_EQUAL);
	CPPUNIT_TEST(testMINUS_EQUAL);
	CPPUNIT_TEST(testMOD_EQUAL);
	CPPUNIT_TEST(testNOT_EQUAL);
	CPPUNIT_TEST(testOR_EQUAL);
	CPPUNIT_TEST(testPLUS_EQUAL);
	CPPUNIT_TEST(testRSHIFT);
	CPPUNIT_TEST(testRSHIFT_EQUAL);
	CPPUNIT_TEST(testTIMES_EQUAL);
	CPPUNIT_TEST(testXOR_EQUAL);
	CPPUNIT_TEST(testRIGHT_SLIM_ARROW);
	CPPUNIT_TEST(testDOUBLE_COLON);
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
	CPPUNIT_TEST(testDOC_COMMENT);
	CPPUNIT_TEST(testBLOCK_COMMENT);
	CPPUNIT_TEST(testCharLiteral);
	CPPUNIT_TEST(testQuoteLiteral);
	CPPUNIT_TEST(testQuoteLiteralHashed);
	CPPUNIT_TEST(testStringLiteral);
	CPPUNIT_TEST(testOptions);
	CPPUNIT_TEST(testSameScope);
	CPPUNIT_TEST(testDifferentScope);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		RustTokenizer ct("crate <>");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_crate), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('<'), ct.get_token());

		RustTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_while), ct2.get_token());

		RustTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct3.get_token());
	}

	void testIdentifier() {
		// Rust doesn't have classes
		RustTokenizer ct("class ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());

		// Raw identifier
		RustTokenizer ct2("r#foo (foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());

		// Raw identifier of reserved word
		RustTokenizer ct3("r#for for(");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct3.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_for), ct3.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct3.get_token());
	}

	void testCharacterToken() {
		RustTokenizer ct("+ - * / =\t<\n> %()[]{}^|&~,.;:!#@$_");
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
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('@'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('$'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('_'), ct.get_token());
	}

	void testAND_EQUAL() {
		RustTokenizer ct("&=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_EQUAL), ct.get_token());
	}

	void testBOOLEAN_AND() {
		RustTokenizer ct("&&b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_AND), ct.get_token());
	}

	void testBOOLEAN_OR() {
		RustTokenizer ct("||b");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_OR), ct.get_token());
	}

	void testDIV_EQUAL() {
		RustTokenizer ct("/=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DIV_EQUAL), ct.get_token());
	}

	void testELIPSIS() {
		RustTokenizer ct("...x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::ELIPSIS), ct.get_token());
	}

	void testDOT_DOT() {
		RustTokenizer ct("..x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DOT_DOT), ct.get_token());
	}

	void testDOT_DOT_EQUAL() {
		RustTokenizer ct("..=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DOT_DOT_EQUAL), ct.get_token());
	}

	void testEQUAL() {
		RustTokenizer ct("==2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::EQUAL), ct.get_token());
	}

	void testRIGHT_FAT_ARROW() {
		RustTokenizer ct("=>a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RIGHT_FAT_ARROW), ct.get_token());
	}

	void testGREATER_EQUAL() {
		RustTokenizer ct(">=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::GREATER_EQUAL), ct.get_token());
	}

	void testLESS_EQUAL() {
		RustTokenizer ct("<=12");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LESS_EQUAL), ct.get_token());
	}

	void testLSHIFT() {
		RustTokenizer ct("<<3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT), ct.get_token());
	}

	void testLSHIFT_EQUAL() {
		RustTokenizer ct("<<=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT_EQUAL), ct.get_token());
	}

	void testMINUS_EQUAL() {
		RustTokenizer ct("-=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MINUS_EQUAL), ct.get_token());
	}

	void testMOD_EQUAL() {
		RustTokenizer ct("%=2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::MOD_EQUAL), ct.get_token());
	}

	void testNOT_EQUAL() {
		RustTokenizer ct("!=3");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL), ct.get_token());
	}

	void testOR_EQUAL() {
		RustTokenizer ct("|=1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::OR_EQUAL), ct.get_token());
	}

	void testPLUS_EQUAL() {
		RustTokenizer ct("+=10");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::PLUS_EQUAL), ct.get_token());
	}

	void testRSHIFT() {
		RustTokenizer ct(">>2");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}
	void testRSHIFT_EQUAL() {
		RustTokenizer ct(">>=5");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RSHIFT_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1502), ct.get_token());
	}

	void testTIMES_EQUAL() {
		RustTokenizer ct("*=4");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::TIMES_EQUAL), ct.get_token());
	}

	void testRIGHT_SLIM_ARROW() {
		RustTokenizer ct("->a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RIGHT_SLIM_ARROW), ct.get_token());
	}

	void testDOUBLE_COLON() {
		RustTokenizer ct("::a");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::DOUBLE_COLON), ct.get_token());
	}

	void testAND() {
		RustTokenizer ct("&&x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_AND), ct.get_token());
	}

	void testBITOR() {
		RustTokenizer ct("|x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('|'), ct.get_token());
	}

	void testOR() {
		RustTokenizer ct("||x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BOOLEAN_OR), ct.get_token());
	}

	void testXOR() {
		RustTokenizer ct("^x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('^'), ct.get_token());
	}

	void testCOMPL() {
		RustTokenizer ct("~x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('~'), ct.get_token());
	}

	void testBITAND() {
		RustTokenizer ct("&x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('&'), ct.get_token());
	}

	void testAND_EQ() {
		RustTokenizer ct("&=x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::AND_EQUAL), ct.get_token());
	}

	void testOR_EQ() {
		RustTokenizer ct("|=x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::OR_EQUAL), ct.get_token());
	}

	void testXOR_EQ() {
		RustTokenizer ct("^=x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::XOR_EQUAL), ct.get_token());
	}

	void testNOT() {
		RustTokenizer ct("!x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('!'), ct.get_token());
	}

	void testNOT_EQ() {
		RustTokenizer ct("!=x");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL), ct.get_token());
	}

	void testXOR_EQUAL() {
		RustTokenizer ct("^=0xff");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::XOR_EQUAL), ct.get_token());
	}

	void testBLOCK_COMMENT() {
		RustTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
	}

	void testDOC_COMMENT() {
		RustTokenizer ct("/** hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_DOC_COMMENT), ct.get_token());
	}

	void testLINE_COMMENT() {
		RustTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
	}

	void testQuoteLiteral() {
		RustTokenizer ct("'a: for");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LABEL_LITERAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_for), ct.get_token());

		RustTokenizer ct2(" fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_fn), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('<'), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LIFETIME_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('>'), ct2.get_token());
	}

	void testQuoteLiteralHashed() {
		RustTokenizer ct("'a: for");
		ct.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LABEL_LITERAL), ct.get_token());
		token_type t1 = ct.get_token();
		CPPUNIT_ASSERT(t1 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_for), ct.get_token());

		RustTokenizer ct2("fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {");
		ct2.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_fn), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('<'), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LIFETIME_LITERAL), ct2.get_token());
		token_type t2 = ct2.get_token();
		CPPUNIT_ASSERT(t2 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('>'), ct2.get_token());
	}

	void testCharLiteral() {
		RustTokenizer ct("'a'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct.get_token());

		RustTokenizer ct2("'\\\'a'+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct2.get_token());

		RustTokenizer ct3("'\\001'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct3.get_token());

		RustTokenizer ct4("'\\xfa'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct4.get_token());

		RustTokenizer ct6("'\n' 'a' '\n'");
		ct6.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct6.get_token());
		token_type t1 = ct6.get_token();
		CPPUNIT_ASSERT(t1 & TokenId::HASHED_CONTENT);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct6.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct6.get_token());
		token_type t2 = ct6.get_token();
		CPPUNIT_ASSERT(t2 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT(t1 != t2);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct6.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct6.get_token());
		token_type t3 = ct6.get_token();
		CPPUNIT_ASSERT(t3 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(t1, t3);

		RustTokenizer ct7("b'a')");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BYTE_LITERAL), ct7.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct7.get_token());

		RustTokenizer ct8("br'a')");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BYTE_LITERAL), ct8.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct8.get_token());

		RustTokenizer ct9("r'a')");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct9.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct9.get_token());

		RustTokenizer ct10("r#'a'#)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct10.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct10.get_token());

		RustTokenizer ct11("r##'\\'##)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct11.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct11.get_token());

		RustTokenizer ct12("br##'\\'##)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BYTE_LITERAL), ct12.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct12.get_token());
		RustTokenizer ct13("r##'\\'#x'##)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct13.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct13.get_token());

	}

	void testStringLiteral() {
		RustTokenizer ct("\"hello\"");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());

		RustTokenizer ct2("\"\\\"a\"+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct2.get_token());

		RustTokenizer ct3("\"\\001\"");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct3.get_token());

		RustTokenizer ct4("\"\\xfa\"");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct4.get_token());

		RustTokenizer ct6("\"hello\n\" \"world\" \"hello\n\"");
		ct6.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct6.get_token());
		token_type t1 = ct6.get_token();
		CPPUNIT_ASSERT(t1 & TokenId::HASHED_CONTENT);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct6.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct6.get_token());
		token_type t2 = ct6.get_token();
		CPPUNIT_ASSERT(t2 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT(t1 != t2);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct6.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct6.get_token());
		token_type t3 = ct6.get_token();
		CPPUNIT_ASSERT(t3 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(t1, t3);

		RustTokenizer ct7("b\"bytes\")");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BYTE_STRING_LITERAL), ct7.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct7.get_token());

		RustTokenizer ct8("br\"raw bytes\\\")");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BYTE_STRING_LITERAL), ct8.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct8.get_token());

		RustTokenizer ct9("r\"raw string\\\")");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct9.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct9.get_token());

		RustTokenizer ct10("r#\"raw delimited\"string\"#)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct10.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct10.get_token());

		RustTokenizer ct11("r##\"\\\"still \"going on\"##)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct11.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct11.get_token());

		RustTokenizer ct12("br##\"\\\"##)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BYTE_STRING_LITERAL), ct12.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct12.get_token());
		RustTokenizer ct13("r##\"\\\"#x\"##)");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct13.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(')'), ct13.get_token());
	}

	void testOptions() {
		RustTokenizer ct("foo ", {"method"});
		CPPUNIT_ASSERT_EQUAL(RustTokenizer::PT_METHOD, ct.get_processing_type());

		RustTokenizer ct2("foo ", {"statement"});
		CPPUNIT_ASSERT_EQUAL(RustTokenizer::PT_STATEMENT, ct2.get_processing_type());
	}

	void testSameScope() {
		RustTokenizer ct("foo { foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
	}

	void testDifferentScope() {
		RustTokenizer ct("{ foo } foo");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('{'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('}'), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER + 1), ct.get_token());
	}

};
#endif /*  RUSTTOKENIZERTEST_H */
