#include "deviceList.hpp"
#include "device_Temperature.hpp"
#include <chrono>
#include <future>

#include <gtest/gtest.h>

using namespace isaac;


TEST(TemperatureSensorDevice, Constructor)
{
	ASSERT_THROW(Temp t1(7), std::invalid_argument);
	ASSERT_THROW(Temp t1(7, ""), std::invalid_argument);
	ASSERT_THROW(Temp t1(7, "", ""), std::invalid_argument);
	ASSERT_THROW(Temp t1(7, "", "", ""), std::invalid_argument);

	ASSERT_NO_THROW(Temp t1(7, "abc", "12345678", "tempSensor"));
	ASSERT_NO_THROW(Temp t1(7, "abc", "12345678", "tempSensor", 1000));
}

TEST(TemperatureSensorDevice, getTemperature)
{
	Temp t1(7, "#123123", "12345678", "tempSensor", 1000);
	ASSERT_EQ(12345, t1.getTemperature());

	std::vector<std::future<int>> futures;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	for (int i = 0; i < 10; ++i) {
		futures.push_back(std::async([&]() { return t1.getTemperature(); }));
	}
	for (auto &result : futures) {
		auto r = result.get();
		EXPECT_EQ(12345, r);
	}
	end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	ASSERT_GT(2, elapsed_seconds.count());
}
