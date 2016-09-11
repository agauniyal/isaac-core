#include <gtest/gtest.h>
#include "device_LED.hpp"

TEST(LEDConstruction, LedDevice)
{
	Led led1(4, "MyLED");
	ASSERT_EQ(false, led1.isBad());
	ASSERT_EQ(false, led1.hasFailed());
	// always returns true for LEDs
	ASSERT_EQ(true, led1.execute());
}

TEST(LEDWrites, LedDevice)
{
	Led led1(4, "MyLED");
	EXPECT_EQ(false, led1.isBad());
	EXPECT_EQ(false, led1.hasFailed());
	// now call on()
	ASSERT_EQ(true, led1.on());
	EXPECT_EQ(false, led1.isBad());
	EXPECT_EQ(false, led1.hasFailed());
}
