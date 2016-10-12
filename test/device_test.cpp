#include "deviceList.hpp"
#include "device_Led.hpp"
#include <fstream>
#include <gtest/gtest.h>

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


TEST(StaticMethods, Device)
{
	Device::configure();
	const auto pinNumber = 7;
	ASSERT_FALSE(Device::isOccupied(pinNumber));

	{
		Led someNewLed(pinNumber, "Red led", "#1231231");
		ASSERT_TRUE(Device::isOccupied(pinNumber));
	}

	ASSERT_FALSE(Device::isOccupied(pinNumber));
}


TEST(Device, dumpInfo)
{

	Led device_sub1(11, "Blue light", "#2222222");
	auto j_info = device_sub1.dumpInfo();

	ASSERT_EQ("Blue light", j_info["name"]);
	ASSERT_EQ(11, (int) j_info.at("powerPin"));
	ASSERT_EQ("#2222222", j_info["id"]);
}


TEST(Device, getType)
{
	std::unique_ptr<Device> d = std::make_unique<Led>(11, "Green Light", "#2222222");
	deviceType type           = deviceType::Base;

	ASSERT_EQ(type, d->Device::getType());
}


TEST(Device, Name)
{
	ASSERT_THROW(Led l1(7), std::invalid_argument);
	ASSERT_THROW(Led l1(7, ""), std::invalid_argument);
	std::string nameSoBig(55, 'a');
	ASSERT_THROW(Led l1(7, nameSoBig), std::invalid_argument);

	Led device_sub2(11, "Blue light", "#2222222");
	ASSERT_EQ("Blue light", device_sub2.getName());

	std::string newName = "Green Light";
	device_sub2.setName(newName);
	ASSERT_EQ(newName, device_sub2.getName());

	device_sub2.setName("");
	ASSERT_EQ(newName, device_sub2.getName());
}


TEST(Device, Id)
{

	ASSERT_THROW(Led l1(7, "a", ""), std::invalid_argument);
	ASSERT_THROW(Led l1(7, "a", "23424"), std::invalid_argument);
	ASSERT_THROW(Led l1(7, "a", "ssdfqweqwexasda"), std::invalid_argument);

	ASSERT_NO_THROW(Led l1(7, "a", "12345678"));

	Led ll(7, "myName", "12345678");
	ASSERT_EQ("12345678", ll.getId());

	// By itself a device has no concept of unique Ids
	// so more than 2 devices can exist with same Ids unless
	// they're inside deviceList container

	Led l1(11, "a", "12345678");
	Led l2(12, "a", "12345678");
}


TEST(Device, Description)
{
	Led device_sub(7, "Green LED", "#1111111");
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


TEST(Device, Exception)
{
	ASSERT_THROW(Led l1(42, "a", "12345678"), std::invalid_argument);
	ASSERT_THROW(Led l1(-42, "a", "12345678"), std::invalid_argument);
	ASSERT_THROW(Led l1(0, "a", "12345678"), std::invalid_argument);
	std::string s(60, 'a');
	ASSERT_THROW(Led l1(7, s, "12345678"), std::invalid_argument);
	ASSERT_THROW(Led l1(8, "a", "12345678"), std::runtime_error);
}
