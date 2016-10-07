#include "deviceList.hpp"
#include "device_Smart_Switch.hpp"
#include <fstream>
#include <gtest/gtest.h>

using namespace isaac;


TEST(SmartSwitchDevice, Constructor)
{
	ASSERT_THROW(SmartSwitch s1(7), std::invalid_argument);
	ASSERT_THROW(SmartSwitch s1(7, ""), std::invalid_argument);
	ASSERT_THROW(SmartSwitch s1(7, "", ""), std::invalid_argument);
	ASSERT_NO_THROW(SmartSwitch s1(7, "abc", "12345678"));
	ASSERT_NO_THROW(SmartSwitch s1(7, "abc", "12345678", 2.0));

	SmartSwitch s1(7, "Switch1", "#2222112");
	EXPECT_FALSE(s1.isOn());
}


TEST(SmartSwitchDevice, OnOff)
{
	SmartSwitch s1(7, "abc", "12345678", 2.0);

	s1.on();
	ASSERT_EQ(true, s1.isOn());

	s1.off();
	ASSERT_EQ(false, s1.isOn());
}


TEST(SmartSwitchDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<SmartSwitch>(7, "abc", "12345678", 2.0);
	deviceType type           = deviceType::SmartSwitch;

	ASSERT_EQ(type, d->getType());
}


TEST(SmartSwitchDevice, runTime)
{
	SmartSwitch s1(7, "Switch1", "#2222112");
	s1.on();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	s1.off();
	ASSERT_GT(2, s1.getRunTime());

	s1.resetRunTime();
	ASSERT_EQ(0, s1.getRunTime());
}


TEST(SmartSwitchDevice, powerConsumption)
{
	SmartSwitch s1(7, "Switch1", "#2222112", 5.0);
	s1.on();
	ASSERT_EQ(5.0, s1.getPowerConsumption());

	s1.setPowerConsumption(4.0);
	ASSERT_EQ(4.0, s1.getPowerConsumption());
}
