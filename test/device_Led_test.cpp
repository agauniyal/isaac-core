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
	std::unique_ptr<Device> d = std::make_unique<Led>(15, "Green Light", "#2222222");
	deviceType type           = deviceType::Led;

	ASSERT_EQ(type, d->getType());
}


TEST(LedDevice, getLastAccessed)
{
	using namespace std::chrono;

	Led l1(11, "greenLed", "#1234567");
	auto epoch = l1.getLastAccessed();

	std::this_thread::sleep_for(milliseconds(5));

	auto epochNow = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	ASSERT_GT(epochNow, epoch);
}
