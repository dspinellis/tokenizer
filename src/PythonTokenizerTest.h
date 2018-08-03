#ifndef PYTHONTOKENIZERTEST_H
#define PYTHONTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "PythonTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class PythonTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(PythonTokenizerTest);
	CPPUNIT_TEST(testSimpleString);
	CPPUNIT_TEST(testEmptyString);
	CPPUNIT_TEST(testRawString);
	CPPUNIT_TEST(testPrefixString);
	CPPUNIT_TEST(testTripleString);
	CPPUNIT_TEST(testTripleEmptyString);
	CPPUNIT_TEST(testComment);
	CPPUNIT_TEST(testRaise);
	CPPUNIT_TEST(testOperatorsDelimiters);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSimpleString() {
		PythonTokenizer ct("a'string' in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());

		PythonTokenizer ct2("a\"string\" in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct2.get_token());
	}

	void testEmptyString() {
		PythonTokenizer ct("a'' in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());

		PythonTokenizer ct2("a\"\" in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct2.get_token());
	}

	void testRawString() {
		PythonTokenizer ct("a r'string' in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());

		PythonTokenizer ct2("a r\"string\" in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct2.get_token());
	}

	void testPrefixString() {
		PythonTokenizer ct("a  r'string' u'string' R'string' U'string' f'string' F'string' fr'string' Fr'string' fR'string' FR'string' rf'string' rF'string' Rf'string' RF'string' b'string' B'string' br'string' Br'string' bR'string' BR'string' rb'string' rB'string' Rb'string' RB'string' in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		for (int i = 0; i < 24; i++)
			CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());
	}

	void testTripleString() {
		PythonTokenizer ct("a'''str''ing\nconti'n\"\"\"ued''' in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());

		PythonTokenizer ct2("a\"\"\"str\"\"ing\n'''conti\"nued\"\"\" in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct2.get_token());
	}

	void testTripleEmptyString() {
		PythonTokenizer ct("a'''''' in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());

		PythonTokenizer ct2("a\"\"\"\"\"\" in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::STRING_LITERAL, ct2.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct2.get_token());
	}

	void testComment() {
		PythonTokenizer ct("a # foo \n in");
		CPPUNIT_ASSERT_EQUAL((int)TokenId::IDENTIFIER, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::LINE_COMMENT, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());
	}

	void testRaise() {
		PythonTokenizer ct("** **= *= *");
		CPPUNIT_ASSERT_EQUAL((int)Token::RAISE, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::RAISE_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)Token::TIMES_EQUAL, ct.get_token());
		CPPUNIT_ASSERT_EQUAL((int)'*', ct.get_token());
	}

	// Test all multi-character operators and delimiters
	void testOperatorsDelimiters() {
		PythonTokenizer ct("!= % %= & &= @ @= ^ ^= | |= = == + += - -= -> * ** **= *= / // //= /= < << <<= <= > >= >> >>= in");
		for (int i = 0; i < 34; i++)
			(void)ct.get_token();
		CPPUNIT_ASSERT_EQUAL((int)Keyword::K_in, ct.get_token());
	}
};
#endif /*  PYTHONTOKENIZERTEST_H */
