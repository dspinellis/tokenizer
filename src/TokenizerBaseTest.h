#ifndef TOKENIZERBASETEST_H
#define TOKENIZERBASETEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CTokenizer.h"
#include "Token.h"
#include "Keyword.h"

/*
 * Use he C tokenizer to test functionality in the TokenizerBase
 * class.
 */
class TokenizerBaseTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(TokenizerBaseTest);
	CPPUNIT_TEST(testCharLiteral);
	CPPUNIT_TEST(testStringLiteral);
	CPPUNIT_TEST(testComment);
	CPPUNIT_TEST(testHashBlockComment);
	CPPUNIT_TEST(testHashLineComment);
	CPPUNIT_TEST(testNumber);
	CPPUNIT_TEST(testOutputLineNumber);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCharLiteral() {
		CTokenizer ct("'a'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct.get_token());

		CTokenizer ct2("'\\\'a'+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct2.get_token());

		CTokenizer ct3("'\\001'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct3.get_token());

		CTokenizer ct4("'\\xfa'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct4.get_token());
		CTokenizer ct5("L'a'");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct5.get_token());

		CTokenizer ct6("'a'");
		ct6.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CHAR_LITERAL), ct6.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('a'), ct6.get_token());

	}

	void testStringLiteral() {
		CTokenizer ct("\"hello\"");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct.get_token());

		CTokenizer ct2("\"he\\\"llo\"+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct2.get_token());

		CTokenizer ct3("L\"hello\"");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct3.get_token());

		CTokenizer ct4("\"hello\" \"world\" \"hello\"");
		ct4.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct4.get_token());
		token_type t1 = ct4.get_token();
		CPPUNIT_ASSERT(t1 & TokenId::HASHED_CONTENT);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct4.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct4.get_token());
		token_type t2 = ct4.get_token();
		CPPUNIT_ASSERT(t2 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT(t1 != t2);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct4.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::STRING_LITERAL), ct4.get_token());
		token_type t3 = ct4.get_token();
		CPPUNIT_ASSERT(t3 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(t1, t3);
	}

	/*
	 * Test comment processing, rather than comment tokens
	 * Comment tokens are tested in the language parsers
	 */
	void testComment() {
		CTokenizer ct(" /* block comment */ +");
		(void)ct.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct.get_token());

		CTokenizer ct2("// line comment\n +");
		(void)ct2.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct2.get_token());

		CTokenizer ct3("/* hi * / */\n+");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct3.get_token());

		CTokenizer ct4("/* hi ***/\n+");
		(void)ct4.get_token();
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct4.get_token());
	}

	void testHashBlockComment() {
		CTokenizer ct(" /* One */ /* Another */ /* One */");
		ct.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
		token_type t1 = ct.get_token();
		CPPUNIT_ASSERT(t1 & TokenId::HASHED_CONTENT);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
		token_type t2 = ct.get_token();
		CPPUNIT_ASSERT(t2 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT(t1 != t2);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
		token_type t3 = ct.get_token();
		CPPUNIT_ASSERT(t3 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(t1, t3);
	}

	void testHashLineComment() {
		CTokenizer ct(" // One\n // Another\n// One\n");
		ct.set_all_contents(true);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
		token_type t1 = ct.get_token();
		CPPUNIT_ASSERT(t1 & TokenId::HASHED_CONTENT);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(' '), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
		token_type t2 = ct.get_token();
		CPPUNIT_ASSERT(t2 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT(t1 != t2);

		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
		token_type t3 = ct.get_token();
		CPPUNIT_ASSERT(t3 & TokenId::HASHED_CONTENT);
		CPPUNIT_ASSERT_EQUAL(t1, t3);
	}

	void testNumber() {
		CTokenizer ct("0");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_ZERO), ct.get_token());

		CTokenizer ct1("1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_ZERO + 1), ct1.get_token());

		CTokenizer ct1a("0x1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_ZERO + 1), ct1a.get_token());

		CTokenizer ct2("1.1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_ZERO + 2), ct2.get_token());

		CTokenizer ct3("10");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_ZERO + 2), ct3.get_token());

		CTokenizer ct3a("100");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_ZERO + 3), ct3a.get_token());

		CTokenizer ct4("0.1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1499), ct4.get_token());

		CTokenizer ct4a(".1");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1499), ct4a.get_token());

		CTokenizer ct5("1.1e300");
		token_type v5 = ct5.get_token();
		CPPUNIT_ASSERT(v5 > TokenId::NUMBER_ZERO);
		CPPUNIT_ASSERT(v5 < 2000);

		CTokenizer ct6("1.1E-300f");
		token_type v6 = ct6.get_token();
		CPPUNIT_ASSERT(v6 < TokenId::NUMBER_ZERO);
		CPPUNIT_ASSERT(v5 > 1000);

		CTokenizer ct7("0.1f+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1499), ct7.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct7.get_token());

		CTokenizer ct8("0.1d+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(1499), ct8.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct8.get_token());

		CTokenizer ct9("1e400");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::NUMBER_INFINITE), ct9.get_token());
	}

	void testOutputLineNumber() {
		CTokenizer ct("0");
		CPPUNIT_ASSERT_EQUAL(1, ct.get_output_line_number());
	}
};
#endif /*  TOKENIZERBASETEST_H */
