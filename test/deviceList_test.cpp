#include "deviceList.hpp"
#include "device_Led.hpp"
#include <fstream>
#include <gtest/gtest.h>

using namespace isaac;


TEST(DeviceList, place)
{
	deviceList list;
	deviceType type = deviceType::Led;

	json j1 = json::object();

	j1["powerPin"] = nullptr;
	j1["name"]     = nullptr;
	ASSERT_FALSE(list.place(type, j1));

	j1["name"]     = "abc";
	j1["powerPin"] = 2;
	ASSERT_EQ(true, list.place(type, j1));
	ASSERT_EQ(1, list.size());

	j1["powerPin"] = 3;
	j1["name"]     = "def";
	ASSERT_EQ(true, list.place(type, j1));
	ASSERT_EQ(2, list.size());

	ASSERT_EQ(2, list.size());

	json j2 = json::object();
	ASSERT_FALSE(list.place(type, j2));

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


TEST(DeviceList, proxyMethods)
{
	deviceList list;
	deviceType type = deviceType::Led;
	json j1         = json::object();

	j1["powerPin"] = 2;
	j1["name"]     = "abc";
	EXPECT_EQ(true, list.place(type, j1));

	j1["powerPin"] = 3;
	j1["name"]     = "def";
	EXPECT_EQ(true, list.place(type, j1));

	ASSERT_EQ(2, list.size());
	ASSERT_LT(1, list.max_size());
	ASSERT_FALSE(list.empty());

	list.clear();
	ASSERT_TRUE(list.empty());
}
