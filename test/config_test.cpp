#include "config.hpp"
#include "deviceList.hpp"
#include "device_Led.hpp"

#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

using namespace isaac;


TEST(Config, gpioNumPins) { ASSERT_EQ(41, config::gpioNumPins); }


TEST(Config, getGPIOBasePath)
{
	std::ifstream configFile("config.json");
	EXPECT_TRUE(configFile.is_open());
	std::stringstream buffer;
	if (configFile) {
		buffer << configFile.rdbuf();
	}
	configFile.close();

	auto json_config = json::parse(buffer);
	auto path        = json_config.at("gpio_path");

	auto PathFromFile = config::getGPIOBasePath();

	ASSERT_EQ(path, PathFromFile);
	ASSERT_NE("/sys/class/gpio/", PathFromFile);
}


TEST(Config, getJsonDBPath)
{
	std::ifstream configFile("config.json");
	EXPECT_TRUE(configFile.is_open());
	std::stringstream buffer;
	if (configFile) {
		buffer << configFile.rdbuf();
	}
	configFile.close();

	auto json_config = json::parse(buffer);
	auto path        = json_config.at("db_path");

	auto PathFromFile = config::getJsonDBPath();

	ASSERT_EQ(path, PathFromFile);
}


TEST(Config, getLogPath)
{
	std::ifstream configFile("config.json");
	EXPECT_TRUE(configFile.is_open());
	std::stringstream buffer;
	if (configFile) {
		buffer << configFile.rdbuf();
	}
	configFile.close();

	auto json_config = json::parse(buffer);
	auto path        = json_config.at("log_path");

	auto PathFromFile = config::getLogPath();

	ASSERT_EQ(path, PathFromFile);
}


TEST(Config, getTempSensor)
{
	std::ifstream configFile("config.json");
	EXPECT_TRUE(configFile.is_open());
	std::stringstream buffer;
	if (configFile) {
		buffer << configFile.rdbuf();
	}
	configFile.close();

	auto json_config = json::parse(buffer);
	auto path        = json_config.at("tempSensor_path");

	auto PathFromFile = config::getTempSensor();

	ASSERT_EQ(path, PathFromFile);
}
