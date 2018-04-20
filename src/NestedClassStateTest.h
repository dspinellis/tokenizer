#ifndef NESTEDCLASSSTATETEST_H
#define NESTEDCLASSSTATETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "NestedClassState.h"

class NestedClassStateTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(NestedClassStateTest);
	CPPUNIT_TEST(testClassTransition);
	CPPUNIT_TEST(testMethodPersist);
	CPPUNIT_TEST(testNestedClassInMethod);
	CPPUNIT_TEST(testNestedClassInClass);
	CPPUNIT_TEST(testCppTemplateFunction);
	CPPUNIT_TEST_SUITE_END();
public:
	void testClassTransition() {
		NestedClassState n;
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_class();
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_open_brace();
		  n.saw_open_brace();
		    CPPUNIT_ASSERT(n.in_method());
		  n.saw_close_brace();
		CPPUNIT_ASSERT(!n.in_method());
	}

	void testMethodPersist() {
		NestedClassState n;
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_class();
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_open_brace();
		  CPPUNIT_ASSERT(!n.in_method());
		  n.saw_open_brace();
		    CPPUNIT_ASSERT(n.in_method());
		    n.saw_open_brace();
		      CPPUNIT_ASSERT(n.in_method());
		    n.saw_close_brace();
		    CPPUNIT_ASSERT(n.in_method());
		    n.saw_close_brace();
		  CPPUNIT_ASSERT(!n.in_method());
	}

	void testNestedClassInMethod() {
		NestedClassState n;
		// Outer
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_class();
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_open_brace();
		  // In class
		  CPPUNIT_ASSERT(!n.in_method());
		  n.saw_open_brace();
		    // In method
		    CPPUNIT_ASSERT(n.in_method());
		    n.saw_open_brace();
		      // Code nested in method
		      CPPUNIT_ASSERT(n.in_method());
		      n.saw_class();
		      CPPUNIT_ASSERT(!n.in_method());
		      n.saw_open_brace();
		        // In inner class
			CPPUNIT_ASSERT(!n.in_method());
			n.saw_open_brace();
			  // In inner class method
			  CPPUNIT_ASSERT(n.in_method());
			  n.saw_open_brace();
			    CPPUNIT_ASSERT(n.in_method());
			  n.saw_close_brace();
			  CPPUNIT_ASSERT(n.in_method());
			n.saw_close_brace();
			// Back to inner class
			CPPUNIT_ASSERT(!n.in_method());
		      n.saw_close_brace();
		      // Back to method
		      CPPUNIT_ASSERT(n.in_method());
		    n.saw_close_brace();
		    // Exit method's code nesting
		    CPPUNIT_ASSERT(n.in_method());
		  n.saw_close_brace();
		  // Back to outer class
		  CPPUNIT_ASSERT(!n.in_method());
	}

	void testNestedClassInClass() {
		NestedClassState n;
		// Outer
		CPPUNIT_ASSERT(!n.in_method());
		n.saw_class();
		n.saw_open_brace();
		  // In class
		  CPPUNIT_ASSERT(!n.in_method());
		  n.saw_class();
		  CPPUNIT_ASSERT(!n.in_method());
		  n.saw_open_brace();
		    // In inner class
		    CPPUNIT_ASSERT(!n.in_method());
		    n.saw_open_brace();
		      // In inner class method
		      CPPUNIT_ASSERT(n.in_method());
		      n.saw_open_brace();
			CPPUNIT_ASSERT(n.in_method());
		      n.saw_close_brace();
		      CPPUNIT_ASSERT(n.in_method());
		    n.saw_close_brace();
		    // Back to inner class
		    CPPUNIT_ASSERT(!n.in_method());
		  n.saw_close_brace();
		  // Back to outer class
		  CPPUNIT_ASSERT(!n.in_method());
	}

	void testCppTemplateFunction() {
		NestedClassState n;
		CPPUNIT_ASSERT(!n.in_method());
		// template <class T>
		n.saw_class();
		n.unsaw_class();
		n.saw_open_brace();
		  CPPUNIT_ASSERT(n.in_method());
		n.saw_close_brace();
		CPPUNIT_ASSERT(!n.in_method());
	}
};
#endif /*  NESTEDCLASSSTATETEST_H */
