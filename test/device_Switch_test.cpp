#include "deviceList.hpp"
#include "device_Switch.hpp"
#include <gtest/gtest.h>

using namespace isaac;


TEST(SwitchDevice, Constructor)
{
	ASSERT_THROW(Switch s1(7), std::invalid_argument);
	ASSERT_THROW(Switch s1(7, ""), std::invalid_argument);
	ASSERT_THROW(Switch s1(7, "", ""), std::invalid_argument);
	ASSERT_NO_THROW(Switch s1(7, "abc", "12345678"));
	ASSERT_NO_THROW(Switch s1(7, "abc", "12345678", 2.0));

	Switch s1(7, "Switch1", "#2222112");
	EXPECT_FALSE(s1.isOn());
}


TEST(SwitchDevice, OnOff)
{
	Switch s1(7, "abc", "12345678", 2.0);

	s1.on();
	ASSERT_EQ(true, s1.isOn());

	s1.off();
	s1.off();  // No effect whatsoever, return immediately
	ASSERT_EQ(false, s1.isOn());
}


TEST(SwitchDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<Switch>(7, "abc", "12345678", 2.0);
	deviceType type           = deviceType::Switch;

	ASSERT_EQ(type, d->getType());
}


TEST(SwitchDevice, runTime)
{
	Switch s1(7, "Switch1", "#2222112");
	s1.on();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	s1.off();
	ASSERT_LT(0, s1.getRunTime());

	s1.resetRunTime();
	ASSERT_EQ(0, s1.getRunTime());
}


TEST(SwitchDevice, powerConsumption)
{
	Switch s1(7, "Switch1", "#2222112", 5.0);
	s1.on();
	ASSERT_EQ(5.0, s1.getPowerConsumption());

	s1.setPowerConsumption(4.0);
	ASSERT_EQ(4.0, s1.getPowerConsumption());
}


TEST(SwitchDevice, dumpInfo)
{
	Switch s1(7, "Switch1", "#2222112", 10.5);
	s1.on();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	s1.off();

	auto jsonInfo = s1.dumpInfo();
	auto type     = dToInt(deviceType::Switch);

	ASSERT_EQ(7, jsonInfo.at("powerPin"));
	ASSERT_EQ("#2222112", jsonInfo.at("id"));
	ASSERT_EQ("Switch1", jsonInfo.at("name"));
	ASSERT_GT(11, jsonInfo.at("powerConsumption"));
	ASSERT_LT(10, jsonInfo.at("powerConsumption"));
	ASSERT_EQ(type, jsonInfo.at("type"));
}


TEST(SwitchDevice, place)
{
	deviceList list;
	deviceType type = deviceType::Switch;

	json j1 = json::object();

	j1["powerPin"]         = nullptr;
	j1["name"]             = nullptr;
	j1["powerConsumption"] = 5.2;
	ASSERT_FALSE(list.place(type, j1).second);

	j1["name"]        = "abc";
	j1["powerPin"]    = 7;
	j1["description"] = "A new Switch device";
	ASSERT_EQ(true, list.place(type, j1).second);
	ASSERT_EQ(1, list.size());

	json j2 = json::object();
	ASSERT_FALSE(list.place(type, j2).second);
}
