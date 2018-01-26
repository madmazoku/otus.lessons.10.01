#define BOOST_TEST_MODULE TestMain
#include <boost/test/unit_test.hpp>

#ifdef __unix__
#include "../bin/version.h"
#else
#include "in.version.h"
#endif

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( test_version )
{
    BOOST_CHECK_GT(build_version(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
