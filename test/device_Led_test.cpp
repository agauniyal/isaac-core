#include "deviceList.hpp"
#include "device_Led.hpp"
#include <fstream>
#include <gtest/gtest.h>

using namespace isaac;


TEST(LedDevice, Constructor)
{
	ASSERT_THROW(Led l1(7), std::invalid_argument);
	ASSERT_THROW(Led l1(7, ""), std::invalid_argument);
	ASSERT_THROW(Led l1(7, "", ""), std::invalid_argument);
	ASSERT_NO_THROW(Led l1(7, "abc", "12345678"));

	Led led1(7, "MyLED", "#2222112");
	EXPECT_FALSE(led1.isOn());
}


TEST(LedDevice, OnOff)
{
	Led led1(11, "Voilet Led", "#3242441");

	led1.on();
	ASSERT_EQ(true, led1.isOn());

	led1.off();
	ASSERT_EQ(false, led1.isOn());
}


TEST(LedDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<Led>(11, "Green Light", "#2222222");
	deviceType type           = deviceType::Led;

	ASSERT_EQ(type, d->getType());
}


TEST(LedDevice, dumpInfo)
{
	Led l1(7, "MyLed", "12345678");
	auto jsonInfo = l1.dumpInfo();
	auto type     = dToInt(deviceType::Led);

	ASSERT_EQ(7, jsonInfo.at("powerPin"));
	ASSERT_EQ("12345678", jsonInfo.at("id"));
	ASSERT_EQ("MyLed", jsonInfo.at("name"));
	ASSERT_EQ(type, jsonInfo.at("type"));
}
