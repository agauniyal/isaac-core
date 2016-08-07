#include "app.hpp"
#include <gtest/gtest.h>

TEST(basic_test, returnsTrue)
{
	ASSERT_EQ(doSomething(), true);
}
