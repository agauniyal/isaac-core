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
	j1["powerPin"] = 7;
	ASSERT_EQ(true, list.place(type, j1));
	ASSERT_EQ(1, list.size());

	j1["powerPin"] = 11;
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
	j1["powerPin"] = 7;
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


TEST(DeviceList, syncmemtodisk)
{
	// TODO: write suitable test
	deviceList list;
	deviceType type = deviceType::Led;
	json j1         = json::object();
	j1["powerPin"]  = 7;
	j1["name"]      = "abc";

	EXPECT_EQ(true, list.place(type, j1));
	ASSERT_THROW(list.sync(1, "abc/bcd.json"), std::runtime_error);
	ASSERT_THROW(list.sync(0, "abc/bcd.json"), std::runtime_error);
	list.place(type, j1);
	list.sync(1, "db1.json");
	std::ifstream db("db1.json");
	if (db) {
		std::stringstream buffer;
		buffer << db.rdbuf();
		auto json_db = json::parse(buffer);
		int a        = json_db[0].at("powerPin");
		ASSERT_EQ(7, a);
	} else {
		ASSERT_EQ(1, 2);
	}
}
TEST(DeviceList, syncdisktomem)
{
	deviceType type = deviceType::Led;
	json j2arr      = json::array();
	json j2         = json::object();
	j2["powerPin"]  = 11;
	j2["name"]      = "bcd";
	j2["type"]      = dToInt(type);
	j2arr.push_back(j2);

	std::ofstream newdb("newdb.json");
	newdb << j2arr.dump(4);
	newdb.close();
	deviceList list2;
	list2.sync(0, "newdb.json");
	ASSERT_EQ(1, list2.size());
}


TEST(DeviceList, proxyMethods)
{
	deviceList list;
	deviceType type = deviceType::Led;
	json j1         = json::object();

	j1["powerPin"] = 7;
	j1["name"]     = "abc";
	EXPECT_EQ(true, list.place(type, j1));

	j1["powerPin"] = 11;
	j1["name"]     = "def";
	EXPECT_EQ(true, list.place(type, j1));

	ASSERT_EQ(2, list.size());
	ASSERT_LT(1, list.max_size());
	ASSERT_FALSE(list.empty());
	ASSERT_EQ(0, list.count("aaaaaaaaa"));
	list.clear();
	ASSERT_TRUE(list.empty());
}
