#include "LEDCLASS.h"
#include<string.h>
#include<iostream>
#include<ctime>
#include<iomanip>





			bool led_class::on(int pin_num)
			{
				gpio_pin(pin_num);
				std::cout<<gpio_export();
				std::cout<<gpio_set_dir("out");
				std::cout<<gpio_set_value(1);
				return true;
			}
			bool led_class::off(int pin_num)
			{
				gpio_pin(pin_num);
				std::cout<<gpio_export();
				std::cout<<gpio_set_dir("out");
				std::cout<<gpio_set_value(0);
				this->m.lock();
				std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
				timeoff=std::chrono::system_clock::to_time_t(now);
				this->m.unlock();
				return true;
			}
			void led_class::lastaccess()
			{
				std::cout<<std::put_time(std::localtime(&timeoff),"%Y-%m-%d %X");
			}



