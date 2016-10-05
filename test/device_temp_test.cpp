#include "device_Temperature.hpp"
#include "deviceList.hpp"
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
	ASSERT_NO_THROW(Temp t1(7, "abc", "12345678", "12345"));
	Temp t1(7, "#123123", "12345678", "hello");
	ASSERT_EQ(2000, t1.getDelay());
	Temp t2(11, "#123123", "12345678", "hello", 2);
	ASSERT_EQ(2, t2.getDelay());
}

TEST(TemperatureSensorDevice, getTemperature)
{
	Temp t1(7, "#123123", "12345678", "hello", 1000);

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start               = std::chrono::system_clock::now();
	std::future<int> f1 = std::async([&]() { return t1.getTemperature(); });
	std::future<int> f2 = std::async([&]() { return t1.getTemperature(); });
	std::future<int> f3 = std::async([&]() { return t1.getTemperature(); });
	int a1              = f3.get();
	int a2              = f2.get();
	int a3              = f1.get();
	end                 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	ASSERT_EQ(1234, a1);
	ASSERT_GT(3, elapsed_seconds.count());
}
