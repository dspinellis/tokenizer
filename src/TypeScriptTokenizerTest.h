#ifndef TYPESCRIPTTOKENIZERTEST_H
#define TYPESCRIPTTOKENIZERTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"
#include "JavaScriptTokenizer.h"
#include "Token.h"
#include "Keyword.h"

class TypeScriptTokenizerTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(TypeScriptTokenizerTest);
	CPPUNIT_TEST(testKeyword);
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

		JavaScriptTokenizer ct4("Boolean (", {}, Keyword::L_TypeScript);
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>(Keyword::K_Boolean), ct4.get_token());
		CPPUNIT_ASSERT_EQUAL(static_cast<token_type>('('), ct4.get_token());
	}
};
#endif /*  TYPESCRIPTTOKENIZERTEST_H */
