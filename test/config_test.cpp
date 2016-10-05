#include "deviceList.hpp"
#include "device_Led.hpp"
#include <fstream>
#include <gtest/gtest.h>

using namespace isaac;


TEST(Config, gpioNumPins) { ASSERT_EQ(41, config::gpioNumPins); }


TEST(Config, configFileNotPresent)
{
	const auto file = "configNotFound.json";
	ASSERT_THROW(config::getGPIOBasePath(file), std::runtime_error);
}


TEST(Config, getGPIOBasePath)
{
	const auto file = "config2.json";
	std::ofstream configFile(file, std::ofstream::trunc);
	ASSERT_TRUE(configFile.is_open());
	if (configFile) {
		configFile << "{\"gpio_path\": \"path/for/testing/\"}\n";
	}
	configFile.close();

	auto PathFromFile = config::getGPIOBasePath(file);
	ASSERT_EQ("path/for/testing/", PathFromFile);
	ASSERT_NE("/sys/class/gpio/", PathFromFile);

	std::remove("config2.json");
}


TEST(Config, getJsonDBPath)
{
	const auto file = "config2.json";
	std::ofstream configFile(file, std::ofstream::trunc);
	ASSERT_TRUE(configFile.is_open());
	if (configFile) {
		configFile << "{\"db_path\": \"path/for/testing/\"}\n";
	}
	configFile.close();

	auto PathFromFile = config::getJsonDBPath(file);
	ASSERT_EQ("path/for/testing/", PathFromFile);

	std::remove("config2.json");
}


TEST(Config, getLogPath)
{
	const auto file = "config2.json";
	std::ofstream configFile(file, std::ofstream::trunc);
	ASSERT_TRUE(configFile.is_open());
	if (configFile) {
		configFile << "{\"log_path\": \"path/for/testing/\"}\n";
	}
	configFile.close();

	auto PathFromFile = config::getLogPath(file);
	ASSERT_EQ("path/for/testing/", PathFromFile);

	std::remove("config2.json");
}


TEST(Config, getTempSensor)
{
	const auto file = "config2.json";
	std::ofstream configFile(file, std::ofstream::trunc);
	ASSERT_TRUE(configFile.is_open());
	if (configFile) {
		configFile << "{\"tempSensor_path\": \"path/for/testing/\"}\n";
	}
	configFile.close();

	auto PathFromFile = config::getTempSensor(file);
	ASSERT_EQ("path/for/testing/", PathFromFile);

	std::remove("config2.json");
}
