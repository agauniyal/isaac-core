#include "deviceList.hpp"
#include "device_FlameSensor.hpp"
#include <future>
#include <gtest/gtest.h>

using namespace isaac;


TEST(FlameSensorDevice, Constructor)
{
	ASSERT_THROW(FlameSensor t1(7), std::invalid_argument);
	ASSERT_THROW(FlameSensor t1(7, ""), std::invalid_argument);
	ASSERT_THROW(FlameSensor t1(7, "", ""), std::invalid_argument);
	ASSERT_THROW(FlameSensor t1(7, "abc", "12345678"), std::runtime_error);
	ASSERT_THROW(FlameSensor t1(7, "FlameSensor#1", "12345678", -1), std::runtime_error);
	ASSERT_NO_THROW(FlameSensor t1(7, "abc", "12345678", 11));

	FlameSensor t1(7, "FlameSensor1", "#2222112", 11);
	EXPECT_FALSE(t1.isOn());
}


TEST(FlameSensorDevice, OnOff)
{
	FlameSensor t1(7, "abc", "12345678", 11);

	t1.on();
	ASSERT_EQ(true, t1.isOn());

	t1.off();
	ASSERT_EQ(false, t1.isOn());
}


TEST(FlameSensorDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<FlameSensor>(7, "abc", "12345678", 11);
	deviceType type           = deviceType::FlameSensor;

	ASSERT_EQ(type, d->getType());
}


TEST(FlameSensorDevice, getLastDetected)
{
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
	  std::chrono::system_clock::now().time_since_epoch());
	std::this_thread::sleep_for(std::chrono::seconds(1));
	FlameSensor t1(7, "FlameSensor1", "#2222112", 11);
	ASSERT_LT(now.count(), t1.getLastDetected());
}


TEST(FlameSensorDevice, detect)
{
	FlameSensor t1(7, "abc", "12345678", 11);
	t1.on();

	t1.detect();
	EXPECT_EQ(false, t1.getState());
}


// TEST(FlameSensorDevice, detectMultiThread)
// {
// 	FlameSensor t1(7, "abc", "12345678", 11);
// 	t1.on();
// 	bool cR = true;

// 	std::vector<std::future<bool>> futures;
// 	for (int i = 0; i < 5; ++i) {
// 		futures.push_back(std::async(std::launch::async, [&]() { t1.detect(); }));
// 	}
// 	ASSERT_EQ(false, cR);
// }


TEST(FlameSensorDevice, dumpInfo)
{
	FlameSensor t1(7, "FlameSensor1", "#2222112", 11);
	t1.on();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	auto jsonInfo = t1.dumpInfo();
	auto type     = dToInt(deviceType::FlameSensor);

	ASSERT_EQ(7, (int) jsonInfo.at("powerPin"));
	ASSERT_EQ(11, (int) jsonInfo.at("dataPin"));
	ASSERT_EQ("#2222112", jsonInfo.at("id"));
	ASSERT_EQ("FlameSensor1", jsonInfo.at("name"));
	ASSERT_EQ(type, (int) jsonInfo.at("type"));

	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
	             std::chrono::system_clock::now().time_since_epoch())
	             .count();
	ASSERT_LT(now, (long)jsonInfo.at("lastDetected"));
}


TEST(FlameSensorDevice, place)
{
	deviceList list;
	deviceType type = deviceType::FlameSensor;

	json j1 = json::object();

	j1["powerPin"] = nullptr;
	j1["name"]     = nullptr;;
	ASSERT_FALSE(list.place(type, j1).second);

	j1["name"]         = "abc";
	j1["powerPin"]     = 11;
	j1["dataPin"]      = 7;
	j1["description"]  = "A new FlameSensor device";
	j1["LastDetected"] = 1476216230093;
	ASSERT_EQ(true, list.place(type, j1).second);
	ASSERT_EQ(1, list.size());

	json j2 = json::object();
	ASSERT_FALSE(list.place(type, j2).second);
}
