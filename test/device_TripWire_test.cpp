#include "deviceList.hpp"
#include "device_TripWire.hpp"
#include <gtest/gtest.h>
#include <future>

using namespace isaac;


TEST(TripWireDevice, Constructor)
{
	ASSERT_THROW(TripWire t1(7), std::invalid_argument);
	ASSERT_THROW(TripWire t1(7, ""), std::invalid_argument);
	ASSERT_THROW(TripWire t1(7, "", ""), std::invalid_argument);
	ASSERT_THROW(TripWire t1(7, "TripWire#1", "12345678", -1), std::invalid_argument);
	ASSERT_NO_THROW(TripWire t1(7, "abc", "12345678"));
	ASSERT_NO_THROW(TripWire t1(7, "abc", "12345678", 500));

	TripWire t1(7, "TripWire1", "#2222112");
	EXPECT_FALSE(t1.active());
}


TEST(TripWireDevice, OnOff)
{
	TripWire t1(7, "abc", "12345678", 200);

	t1.on();
	ASSERT_EQ(true, t1.active());

	t1.off();
	ASSERT_EQ(false, t1.active());
}


TEST(TripWireDevice, getType)
{
	std::unique_ptr<Device> d = std::make_unique<TripWire>(7, "abc", "12345678", 500);
	deviceType type           = deviceType::TripWire;

	ASSERT_EQ(type, d->getType());
}


TEST(TripWireDevice, getLastBreak)
{
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
	  std::chrono::system_clock::now().time_since_epoch());
	std::this_thread::sleep_for(std::chrono::seconds(1));
	TripWire t1(7, "TripWire1", "#2222112", 500);
	ASSERT_LT(now.count(), t1.getLastBreak());
}


TEST(TripWireDevice, intrusion)
{
	TripWire t1(7, "abc", "12345678", 100);
	t1.on();

	bool result = t1.intrusion();
	ASSERT_EQ(true, result);
}


TEST(TripWireDevice, intrusionMultiThread)
{
	TripWire t1(7, "abc", "12345678", 5000);
	t1.on();
	bool cR = true;

	std::vector<std::future<bool>> futures;
	for (int i = 0; i < 5; ++i) {
		futures.push_back(std::async(std::launch::async, [&]() { return t1.intrusion(); }));
	}
	for (auto &result : futures) {
		bool r = result.get();
		cR = cR && r;
	}
	ASSERT_EQ(false, cR);
}


TEST(TripWireDevice, dumpInfo)
{
	TripWire t1(7, "TripWire1", "#2222112", 100);
	t1.on();

	auto jsonInfo = t1.dumpInfo();
	auto type     = dToInt(deviceType::TripWire);

	ASSERT_EQ(7, (int) jsonInfo.at("powerPin"));
	ASSERT_EQ("#2222112", jsonInfo.at("id"));
	ASSERT_EQ("TripWire1", jsonInfo.at("name"));
	ASSERT_EQ(100, (int) jsonInfo.at("maxCycles"));
	ASSERT_EQ(type, (int) jsonInfo.at("type"));
}


TEST(TripWireDevice, place)
{
	deviceList list;
	deviceType type = deviceType::TripWire;

	json j1 = json::object();

	j1["powerPin"]  = nullptr;
	j1["name"]      = nullptr;
	j1["maxCycles"] = 200;
	ASSERT_FALSE(list.place(type, j1).second);

	j1["name"]        = "abc";
	j1["powerPin"]    = 7;
	j1["description"] = "A new TripWire device";
	j1["lastBreak"]   = 1476216230093;
	ASSERT_EQ(true, list.place(type, j1).second);
	ASSERT_EQ(1, list.size());

	json j2 = json::object();
	ASSERT_FALSE(list.place(type, j2).second);
}
