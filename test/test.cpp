#include <gtest/gtest.h>
#include "device_LED.hpp"
#include <fstream>

using namespace isaac;

/**
 ** NOTE: Add 100ms delay after device/child constructor is called
 **		  on RPi or simillar boards if tests concerning constructor
 **		  or immediate writes fail. That is not an error, it takes time
 **		  to populate hardware mapped directories/files on such devices.
 **
 ** CODE:
 **			#include <chrono>
 **			#include <thread>
 **			using namespace std::literals::chrono_literals;
 **			std::this_thread::sleep_for(100ms);
 **
*/


TEST(getGPIOBasePath, configFilePresent)
{
	const auto file = "config2.json";
	std::ofstream configFile(file, std::ofstream::trunc);
	ASSERT_TRUE(configFile.is_open());
	if (configFile) {
		configFile << "{\"path\": \"path/for/testing\"}\n";
	}
	configFile.close();

	auto PathFromFile = gpio::getGPIOBasePath(file);
	ASSERT_EQ("path/for/testing", PathFromFile);
	ASSERT_NE("/sys/class/gpio", PathFromFile);

	std::remove("config2.json");
}

TEST(getGPIOBasePath, configFileNotPresent)
{
	const auto file = "configNotFound.json";
	ASSERT_THROW(gpio::getGPIOBasePath(file), std::runtime_error);
}

TEST(StaticMethods, Device)
{
	const unsigned pinNumber = 2;
	ASSERT_FALSE(Device::isOccupied(pinNumber));

	// now let's assign a device with pinNumber 2
	{
		Led someNewLed(pinNumber, "Red led");
		ASSERT_TRUE(Device::isOccupied(pinNumber));
	}
	ASSERT_FALSE(Device::isOccupied(pinNumber));
}

TEST(Device, Name)
{
	Led device_sub1(4);
	ASSERT_EQ("DEFAULT DEVICE NAME", device_sub1.getName());

	Led device_sub2(5, "Blue light");
	ASSERT_EQ("Blue light", device_sub2.getName());

	std::string newName = "Green Light";
	device_sub2.setName(newName);
	ASSERT_EQ(newName, device_sub2.getName());
}

TEST(Device, Info)
{
	Led device_sub(6, "Green LED");
	EXPECT_EQ("Green LED", device_sub.getName());

	ASSERT_EQ("null", device_sub.getInfo().dump());

	json newInfo = { { "manufacturer", "Company 1" }, { "consumption", "10W" } };
	device_sub.setInfo(newInfo);
	ASSERT_EQ(newInfo, device_sub.getInfo());

	json modifyInfo      = device_sub.getInfo();
	modifyInfo["colour"] = "green";
	device_sub.setInfo(modifyInfo);
	ASSERT_EQ(modifyInfo, device_sub.getInfo());
}

TEST(LedDevice, Constructor)
{
	Led led1(7, "MyLED");
	EXPECT_FALSE(led1.hasFailed());
	EXPECT_FALSE(led1.isBad());

	// always returns true for LEDs
	ASSERT_TRUE(led1.execute());

	EXPECT_FALSE(led1.isOn());

	// Empty method for now
	led1.process();
}

TEST(LedDevice, OnOff)
{
	Led led1(8, "Booms");
	EXPECT_FALSE(led1.isBad());

	// now call on()
	ASSERT_EQ(true, led1.on());
	EXPECT_FALSE(led1.isBad());

	// now call off()
	ASSERT_EQ(true, led1.off());
	EXPECT_FALSE(led1.isBad());
}

// TEST(Brute, Lifecycle)
// {
// 	for (int i = 0; i < 500; ++i) {
// 		{
// 			Led led1(1, "MyLED");
// 			Led led2(2, "MyLED");
// 			Led led3(3, "MyLED");
// 			Led led4(4, "MyLED");
// 			Led led5(5, "MyLED");

// 			EXPECT_TRUE(led1.on());
// 			EXPECT_TRUE(led2.on());
// 			EXPECT_TRUE(led3.on());
// 			EXPECT_TRUE(led4.on());
// 			EXPECT_TRUE(led5.on());

// 			EXPECT_TRUE(led1.off());
// 			EXPECT_TRUE(led2.off());
// 			EXPECT_TRUE(led3.off());
// 			EXPECT_TRUE(led4.off());
// 			EXPECT_TRUE(led5.off());
// 		}
// 	}
// }
