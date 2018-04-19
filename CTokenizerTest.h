#ifndef CTOKENIZERTEST_H
#define CTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "CTokenizer.h"
#include "CToken.h"
#include "CKeyword.h"

class CTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(CTokenizerTest);
	CPPUNIT_TEST(testKeyword);
	CPPUNIT_TEST_SUITE_END();
public:
	void testKeyword() {
		CTokenizer ct("do ");
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::DO, ct.get_token());

		CTokenizer ct2("while");
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::WHILE, ct2.get_token());

		CTokenizer ct3(";if");
		(void)ct3.get_token();
		CPPUNIT_ASSERT_EQUAL((int)CKeyword::IF, ct3.get_token());
	}
};
#endif /*  CTOKENIZERTEST_H */
