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
		// std::cout << (int)CKeyword::DO << std::endl;
		CPPUNIT_ASSERT_EQUAL(ct.get_token(), (int)CKeyword::DO);
		CTokenizer ct2("do");
		CPPUNIT_ASSERT_EQUAL(ct2.get_token(), (int)CKeyword::DO);
	}
};
#endif /*  CTOKENIZERTEST_H */
