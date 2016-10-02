#include "device_temp.hpp"
#include "deviceList.hpp"

#include <gtest/gtest.h>

using namespace isaac;



TEST(TemperatureSensorDevice, Constructor)
{
	ASSERT_THROW(Temp t1(7), std::invalid_argument);
	ASSERT_THROW(Temp t1(7, ""), std::invalid_argument);
	ASSERT_THROW(Temp t1(7, "", ""), std::invalid_argument);
	ASSERT_THROW(Temp t1(7, "", "",""), std::invalid_argument);
	ASSERT_NO_THROW(Temp t1(7, "abc", "12345678","12345"));
	Temp t1(7,"#123123","12345678","hello");
	ASSERT_EQ(2000,t1.getDelay());
	Temp t2(11,"#123123","12345678","hello",2);
	ASSERT_EQ(2,t2.getDelay());


	}

TEST(TemperatureSensorDevice,getTemperature )
{
	Temp t1(7,"#123123","12345678","hello",2);
	ASSERT_EQ(1234,t1.getTemperature());

}


