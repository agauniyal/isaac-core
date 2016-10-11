#include "deviceList.hpp"
#include "device_Led.hpp"
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


TEST(LedDevice, place)
{
	deviceList list;
	deviceType type = deviceType::Led;

	json j1 = json::object();

	j1["powerPin"] = nullptr;
	ASSERT_FALSE(list.place(type, j1).second);

	j1["name"]        = "abc";
	j1["powerPin"]    = 7;
	j1["description"] = "A new Led device";
	ASSERT_EQ(true, list.place(type, j1).second);
	ASSERT_EQ(1, list.size());

	json j2 = json::object();
	ASSERT_FALSE(list.place(type, j2).second);
}
