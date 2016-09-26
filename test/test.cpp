#include <gtest/gtest.h>
#include "device_LED.hpp"
#include "deviceList.hpp"
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
	auto defaultParamPath = config::getGPIOBasePath();
	ASSERT_EQ("debug/raspi/path/", defaultParamPath);
	ASSERT_NE("/sys/class/gpio/", defaultParamPath);


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

TEST(getGPIOBasePath, configFileNotPresent)
{
	const auto file = "configNotFound.json";
	ASSERT_THROW(config::getGPIOBasePath(file), std::runtime_error);
}

TEST(StaticMethods, Device)
{
	Device::configure();
	const unsigned pinNumber = 2;
	ASSERT_FALSE(Device::isOccupied(pinNumber));

	{
		Led someNewLed(pinNumber, "Red led", "#1231231");
		ASSERT_TRUE(Device::isOccupied(pinNumber));
	}

	ASSERT_FALSE(Device::isOccupied(pinNumber));
}


TEST(Device, dumpInfo)
{

	Led device_sub1(5, "Blue light", "#2222222");
	auto j_info = device_sub1.dumpInfo();

	ASSERT_EQ("Blue light", j_info["name"]);
	int pin = j_info["powerPin"];
	ASSERT_EQ(5, pin);
	ASSERT_EQ("#2222222", j_info["id"]);
}


TEST(Device, getType)
{
	std::unique_ptr<Device> d = std::make_unique<Led>(6, "Green Light", "#2222222");
	deviceType type           = deviceType::Base;

	ASSERT_EQ(type, d->Device::getType());
}


TEST(Device, Name)
{
	ASSERT_THROW(Led l1(2), std::invalid_argument);
	ASSERT_THROW(Led l1(2, ""), std::invalid_argument);
	std::string nameSoBig(55, 'a');
	ASSERT_THROW(Led l1(2, nameSoBig), std::invalid_argument);

	Led device_sub2(5, "Blue light", "#2222222");
	ASSERT_EQ("Blue light", device_sub2.getName());

	std::string newName = "Green Light";
	device_sub2.setName(newName);
	ASSERT_EQ(newName, device_sub2.getName());

	device_sub2.setName("");
	ASSERT_EQ(newName, device_sub2.getName());
}

TEST(Device, Id)
{

	ASSERT_THROW(Led l1(2, "a", ""), std::invalid_argument);
	ASSERT_THROW(Led l1(2, "a", "23424"), std::invalid_argument);
	ASSERT_THROW(Led l1(2, "a", "ssdfqweqwexasda"), std::invalid_argument);

	ASSERT_NO_THROW(Led l1(2, "a", "12345678"));

	Led ll(5, "myName", "12345678");
	ASSERT_EQ("12345678", ll.getId());

	// By itself a device has no concept of unique Ids
	// so more than 2 devices can exist with same Ids unless
	// they're inside deviceList container

	Led l1(3, "a", "12345678");
	Led l2(4, "a", "12345678");
}

TEST(Device, Description)
{
	Led device_sub(6, "Green LED", "#1111111");
	EXPECT_EQ("Green LED", device_sub.getName());

	ASSERT_EQ("null", device_sub.getDescription().dump());

	json newDescription = { { "manufacturer", "Company 1" }, { "consumption", "10W" } };
	device_sub.setDescription(newDescription);
	ASSERT_EQ(newDescription, device_sub.getDescription());

	json modifyDescription      = device_sub.getDescription();
	modifyDescription["colour"] = "green";
	device_sub.setDescription(modifyDescription);
	ASSERT_EQ(modifyDescription, device_sub.getDescription());
}

TEST(LedDevice, Constructor)
{
	ASSERT_THROW(Led l1(2), std::invalid_argument);
	ASSERT_THROW(Led l1(2, ""), std::invalid_argument);
	ASSERT_THROW(Led l1(2, "", ""), std::invalid_argument);
	ASSERT_NO_THROW(Led l1(2, "abc", "12345678"));

	Led led1(7, "MyLED", "#2222112");

	// always returns true for LEDs
	ASSERT_TRUE(led1.execute());

	EXPECT_FALSE(led1.isOn());

	// Empty method for now
	led1.process();
}

TEST(LedDevice, OnOff)
{
	Led led1(8, "Voilet Led", "#3242441");

	led1.on();
	ASSERT_EQ(true, led1.isOn());

	led1.off();
	ASSERT_EQ(false, led1.isOn());
}


TEST(LedDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<Led>(6, "Green Light", "#2222222");
	deviceType type           = deviceType::Led;

	ASSERT_EQ(type, d->getType());
}


TEST(LedDevice, getLastAccessed)
{
	// TODO: write suitable test
	Led l1(3, "greenLed", "#1234567");
	auto epoch = l1.getLastAccessed();
}


TEST(DeviceList, place)
{
	deviceList list;
	deviceType type = deviceType::Led;

	json j1 = json::object();

	j1["powerPin"] = nullptr;
	j1["name"]     = nullptr;
	ASSERT_FALSE(list.place(type, j1));

	j1["powerPin"] = 240;
	j1["name"]     = "abc";
	ASSERT_THROW(list.place(type, j1), std::invalid_argument);

	j1["powerPin"] = 2;
	ASSERT_EQ(true, list.place(type, j1));
	ASSERT_EQ(1, list.size());

	j1["powerPin"] = 3;
	j1["name"]     = "def";
	ASSERT_EQ(true, list.place(type, j1));
	ASSERT_EQ(2, list.size());

	ASSERT_THROW(list.place(type, j1), std::runtime_error);
	ASSERT_EQ(2, list.size());

	deviceType uncompatibleType = deviceType::Base;
	ASSERT_FALSE(list.place(uncompatibleType, j1));
}


TEST(DeviceList, removeId)
{
	deviceList list;
	deviceType type = deviceType::Led;

	json j1        = json::object();
	j1["powerPin"] = 2;
	j1["name"]     = "abc";

	ASSERT_EQ(true, list.place(type, j1));
	ASSERT_EQ(1, list.size());

	auto all = list.getAll();
	ASSERT_EQ(1, all.size());

	auto id = all[0].first;
	ASSERT_EQ(true, list.removeId(id));
	ASSERT_EQ(0, list.size());

	ASSERT_NE(true, list.removeId("1"));
}


TEST(DeviceList, sync)
{
	// TODO: write suitable test
	deviceList list;
	deviceType type = deviceType::Led;
	json j1         = json::object();
	j1["powerPin"]  = 2;
	j1["name"]      = "abc";

	EXPECT_EQ(true, list.place(type, j1));
	list.sync();
}
