#include "deviceList.hpp"
#include "device_Temperature.hpp"
#include <chrono>
#include <future>
#include <gtest/gtest.h>

using namespace isaac;


TEST(TemperatureSensorDevice, Constructor)
{
	ASSERT_THROW(TempSensor t1(7), std::invalid_argument);
	ASSERT_THROW(TempSensor t1(7, ""), std::invalid_argument);
	ASSERT_THROW(TempSensor t1(7, "", ""), std::invalid_argument);
	ASSERT_THROW(TempSensor t1(7, "", "", ""), std::invalid_argument);

	ASSERT_NO_THROW(TempSensor t1(7, "abc", "12345678", "tempSensor"));
	ASSERT_NO_THROW(TempSensor t1(7, "abc", "12345678", "tempSensor", 1000));
}


TEST(TemperatureSensorDevice, getTemperature)
{
	TempSensor t1(7, "#123123", "12345678", "tempSensor");
	ASSERT_EQ(12345, t1.getTemperature());
}


TEST(TemperatureSensorDevice, getTemperatureMultiThread)
{
	TempSensor t1(7, "#123123", "12345678", "tempSensor", 1000);

	std::vector<std::future<int>> futures;
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	start = std::chrono::steady_clock::now();

	for (int i = 0; i < 10; ++i) {
		futures.push_back(std::async(std::launch::async, [&]() { return t1.getTemperature(); }));
	}
	for (auto &result : futures) {
		auto r = result.get();
		EXPECT_EQ(12345, r);
	}
	end = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	ASSERT_GT(2, elapsed_seconds.count());
}


TEST(TemperatureSensorDevice, getType)
{
	std::unique_ptr<Device> d
	  = std::make_unique<TempSensor>(7, "abc", "12345678", "tempSensor", 1000);
	deviceType type = deviceType::TempSensor;

	ASSERT_EQ(type, d->getType());
}


TEST(TemperatureSensorDevice, dumpInfo)
{
	TempSensor t1(7, "#123123", "12345678", "tempSensor");
	auto jsonInfo = t1.dumpInfo();
	auto type     = dToInt(deviceType::TempSensor);

	ASSERT_EQ(7, jsonInfo.at("powerPin"));
	ASSERT_EQ("12345678", jsonInfo.at("id"));
	ASSERT_EQ("#123123", jsonInfo.at("name"));
	ASSERT_EQ(0, jsonInfo.at("delayTime"));
	ASSERT_EQ("tempSensor", jsonInfo.at("folderName"));
	ASSERT_EQ(type, jsonInfo.at("type"));
}


TEST(TemperatureSensorDevice, place)
{
	deviceList list;
	deviceType type = deviceType::TempSensor;

	json j1 = json::object();

	j1["powerPin"]   = nullptr;
	j1["name"]       = nullptr;
	j1["folderName"] = nullptr;

	ASSERT_FALSE(list.place(type, j1).second);

	j1["name"]        = "abc";
	j1["powerPin"]    = 7;
	j1["folderName"]  = "tempSensor";
	j1["delayTime"]   = 200;
	j1["description"] = "A new TemperatureSensor device";
	ASSERT_EQ(true, list.place(type, j1).second);
	ASSERT_EQ(1, list.size());

	// json j2 = json::object();
	// ASSERT_FALSE(list.place(type, j2).second);
}
