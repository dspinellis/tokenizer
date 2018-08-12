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
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_if, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'(', ct.get_token());
	}

	void testIdentifier() {
		PHPTokenizer ct("foo ");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());

		PHPTokenizer ct2("$foo ");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());

		PHPTokenizer ct3("\\a\\b\\c + ");
		CPPUNIT_ASSERT((int)TokenId::is_identifier(ct3.get_token()));
		CPPUNIT_ASSERT_EQUAL((int)'+', ct3.get_token());

		PHPTokenizer ct4("aa\\bbb\\c - ");
		CPPUNIT_ASSERT((int)TokenId::is_identifier(ct4.get_token()));
		CPPUNIT_ASSERT_EQUAL((int)'-', ct4.get_token());
	}

	void testOperator() {
		PHPTokenizer ct("<=> -> ?? === == !== != **= ** *= <= <<= << ... .=");
		CPPUNIT_ASSERT_EQUAL((int)Token::SPACESHIP, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::ARROW, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::NULL_COALESCE, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::IDENTICAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_IDENTICAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::NOT_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::RAISE_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::RAISE, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::TIMES_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::LESS_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::LSHIFT, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::ELIPSIS, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::CONCAT_EQUALS, ct.get_token());
	}


	void testCharacterToken() {
		PHPTokenizer ct("? + - * / =\t<\n> %()[]{}^|&~,.;:!#");
		CPPUNIT_ASSERT_EQUAL((int)'?', ct.get_token());
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
	}

	void testBlockComment() {
		PHPTokenizer ct("/* hi */");
		CPPUNIT_ASSERT_EQUAL((int)Token::BLOCK_COMMENT, ct.get_token());
	}

	void testLineComment() {
		PHPTokenizer ct("// hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct.get_token());
		PHPTokenizer ct2("# hi\n");
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct2.get_token());
	}

	void testHereDocument() {
		PHPTokenizer ct("$a = <<< EOF\nfoo\nEOFA\nx EOF\nEOF;\n+");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'=', ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::HERE_DOCUMENT, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'+', ct.get_token());

		PHPTokenizer ct2("$a = <<< \"EOF\"\nfoo\nEOFA\nx EOF\nEOF;\n+");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'=', ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::HERE_DOCUMENT, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'+', ct2.get_token());
	}
};
#endif /*  PHPTOKENIZERTEST_H */
