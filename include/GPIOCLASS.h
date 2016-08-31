
#include<string>


/* GPIO CLASS
* Each object from this class wil control a single GPIO pin
* Pin nukber must be passes to the overloaded class constructor
*/

class gpio
{
public:
	void gpio_pin(int pin_num);
	bool gpio_export();				 //Exports GPIO
	bool gpio_unexport();			 //Unexports GPIO
	bool gpio_set_dir(std::string DIR);	 //Set GPIO direction
	bool gpio_set_value(int VAL);  //Set GPIO value
	bool gpio_get_value(); //Get GPIO value
	int gpio_get_num();           //Returns GPIO number associated with the instance of an object
private:
	int gpio_pin_num;			 //GPIO number associated with the instance of the object
};
