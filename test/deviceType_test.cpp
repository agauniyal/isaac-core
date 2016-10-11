#include "deviceType.hpp"
#include <gtest/gtest.h>

using namespace isaac;


TEST(DeviceType, dToInt)
{
	ASSERT_EQ(0, dToInt(deviceType::Base));
	ASSERT_EQ(1, dToInt(deviceType::Led));
	ASSERT_EQ(2, dToInt(deviceType::TempSensor));
	ASSERT_EQ(3, dToInt(deviceType::Switch));
	ASSERT_EQ(4, dToInt(deviceType::TripWire));
	ASSERT_EQ(5, dToInt(deviceType::Buzzer));
}


TEST(DeviceType, intToD)
{
	ASSERT_EQ(deviceType::Base, intToD(0));
	ASSERT_EQ(deviceType::Led, intToD(1));
	ASSERT_EQ(deviceType::TempSensor, intToD(2));
	ASSERT_EQ(deviceType::Switch, intToD(3));
	ASSERT_EQ(deviceType::TripWire, intToD(4));
	ASSERT_EQ(deviceType::Buzzer, intToD(5));
}
