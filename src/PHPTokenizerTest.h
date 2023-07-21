#ifndef PHPTOKENIZERTEST_H
#define PHPTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "PHPTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class PHPTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(PHPTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST(testIdentifier);
	CPPUNIT_TEST(testCharacterToken);
	CPPUNIT_TEST(testOperator);
	CPPUNIT_TEST(testBlockComment);
	CPPUNIT_TEST(testLineComment);
	CPPUNIT_TEST(testHereDocument);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		PHPTokenizer ct("if IF If (");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_if), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct.get_token());
	}

	void testIdentifier() {
		PHPTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());

		PHPTokenizer ct2("$foo ");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());

		PHPTokenizer ct3("\\a\\b\\c + ");
		CPPUNIT_ASSERT(TokenId::is_identifier(ct3.get_token()));
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct3.get_token());

		PHPTokenizer ct4("aa\\bbb\\c - ");
		CPPUNIT_ASSERT(TokenId::is_identifier(ct4.get_token()));
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('-'), ct4.get_token());
	}

	void testOperator() {
		PHPTokenizer ct("<=> -> ?? === == !== != **= ** *= <= <<= << ... .=");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::SPACESHIP), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::ARROW), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NULL_COALESCE), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::IDENTICAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_IDENTICAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::NOT_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RAISE_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::RAISE), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::TIMES_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LESS_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT_EQUAL), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LSHIFT), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::ELIPSIS), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::CONCAT_EQUALS), ct.get_token());
	}


	void testCharacterToken() {
		PHPTokenizer ct("? + - * / =\t<\n> %()[]{}^|&~,.;:!#");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('?'), ct.get_token());
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
	}

	void testBlockComment() {
		PHPTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::BLOCK_COMMENT), ct.get_token());
	}

	void testLineComment() {
		PHPTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct.get_token());
		PHPTokenizer ct2("# hi\n");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::LINE_COMMENT), ct2.get_token());
	}

	void testHereDocument() {
		PHPTokenizer ct("$a = <<< EOF\nfoo\nEOFA\nx EOF\nEOF;\n+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('='), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::HERE_DOCUMENT), ct.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct.get_token());

		PHPTokenizer ct2("$a = <<< \"EOF\"\nfoo\nEOFA\nx EOF\nEOF;\n+");
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(TokenId::FIRST_IDENTIFIER), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('='), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Token::HERE_DOCUMENT), ct2.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('+'), ct2.get_token());
	}
};
#endif /*  PHPTOKENIZERTEST_H */
