
#ifndef _YONMAP_TEST_CASE_
#define _YONMAP_TEST_CASE_

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "Yon.lib")

#include <cppunit/extensions/HelperMacros.h>
#include "yon.h"

using namespace yon;
using namespace yon::core;

class yonMapTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( yonMapTestCase );
	CPPUNIT_TEST( size );
	CPPUNIT_TEST( find );
	CPPUNIT_TEST( insert );
	CPPUNIT_TEST( remove );
	CPPUNIT_TEST( insertAndRemove );
	CPPUNIT_TEST_SUITE_END();
protected:
	map<s32,c8> m;

public:
	void setUp();

protected:
	void size();
	void find();
	void insert();
	void remove();
	void insertAndRemove();
};

#endif