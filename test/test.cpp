#include <gtest/gtest.h>
#include "device_LED.hpp"
#include "utils.hpp"
#include <fstream>
#include <cstdio>

TEST(getGPIOBasePath, configFilePresent)
{
	const auto file = "config2.txt";
	std::ofstream configFile(file, std::ofstream::trunc);
	ASSERT_TRUE(configFile.is_open());
	if (configFile) {
		configFile << "{\"path\": \"path/for/testing\"}\n";
	}
	configFile.close();

	auto PathFromFile = gpio::getGPIOBasePath(file);
	ASSERT_EQ("path/for/testing", PathFromFile);
	ASSERT_NE("/sys/class/gpio", PathFromFile);

	std::remove("config2.txt");
}

TEST(getGPIOBasePath, configFileNotPresent)
{
	const auto file  = "configNotFound.txt";
	auto DefaultPath = gpio::getGPIOBasePath(file);
	ASSERT_EQ("debug/raspi/path", DefaultPath);
	ASSERT_NE("/sys/class/gpio", DefaultPath);
}

TEST(LedDevice, Constructor)
{
	Led led1(4, "MyLED");
	ASSERT_FALSE(led1.isBad());
	ASSERT_FALSE(led1.hasFailed());
	// always returns true for LEDs
	ASSERT_TRUE(led1.execute());
}

TEST(LedDevice, On)
{
	Led led1(5, "MyLED");
	EXPECT_FALSE(led1.isBad());
	EXPECT_FALSE(led1.hasFailed());
	// now call on()
	// should add 100ms delay here to pass test on rpi
	ASSERT_EQ(true, led1.on());
	EXPECT_FALSE(led1.isBad());
	EXPECT_FALSE(led1.hasFailed());
}
