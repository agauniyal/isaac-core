#include "deviceList.hpp"
#include "device_Buzzer.hpp"
#include <gtest/gtest.h>

using namespace isaac;


TEST(BuzzerDevice, Constructor)
{
	ASSERT_THROW(Buzzer b1(7), std::invalid_argument);
	ASSERT_THROW(Buzzer b1(7, ""), std::invalid_argument);
	ASSERT_THROW(Buzzer b1(7, "", ""), std::invalid_argument);
	ASSERT_NO_THROW(Buzzer b1(7, "abc", "12345678"));

	Buzzer b2(7, "MyBuzzer", "#2222112");
	EXPECT_FALSE(b2.isOn());
}


TEST(BuzzerDevice, OnOff)
{
	Buzzer b2(11, "Security Buzzer", "#3242441");

	b2.on();
	ASSERT_EQ(true, b2.isOn());

	b2.off();
	ASSERT_EQ(false, b2.isOn());
}


TEST(BuzzerDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<Buzzer>(11, "Big Buzzer", "#2222222");
	deviceType type           = deviceType::Buzzer;

	ASSERT_EQ(type, d->getType());
}


TEST(BuzzerDevice, dumpInfo)
{
	Buzzer b1(7, "MyBuzzer", "12345678");
	auto jsonInfo = b1.dumpInfo();
	auto type     = dToInt(deviceType::Buzzer);

	ASSERT_EQ(7, jsonInfo.at("powerPin"));
	ASSERT_EQ("12345678", jsonInfo.at("id"));
	ASSERT_EQ("MyBuzzer", jsonInfo.at("name"));
	ASSERT_EQ(type, jsonInfo.at("type"));
}


TEST(BuzzerDevice, place)
{
	deviceList list;
	deviceType type = deviceType::Buzzer;

	json j1 = json::object();

	j1["powerPin"] = nullptr;
	j1["name"]     = nullptr;
	ASSERT_FALSE(list.place(type, j1).second);

	j1["name"]        = "abc";
	j1["powerPin"]    = 7;
	j1["description"] = "A new Buzzer device";
	ASSERT_EQ(true, list.place(type, j1).second);
	ASSERT_EQ(1, list.size());

	json j2 = json::object();
	ASSERT_FALSE(list.place(type, j2).second);
}
