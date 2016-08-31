#include<ctime>
#include "GPIOCLASS.h"
#include<mutex>
using namespace std;
class led_class:public gpio
{
	public:
			bool on(int pin_num);
			bool off(int pin_num);
			void lastaccess();
	private:
			time_t timeoff;
			mutex m;

};
