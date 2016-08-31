#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
#include "GPIOCLASS.h"





void gpio::gpio_pin(int pin_num)
{
	gpio_pin_num=pin_num;	//Initiate gpio object for gpio_pin_num "GNUM"

}

bool gpio::gpio_export()
{	std::string line;
	std::fstream config("config.txt");
	if(config.fail())
	{
		std::cout<<"cannot open file";
		return false;
	}
	config>>line;
	std::string export_path;
	export_path.append("/export");
	config<<line;
	std::ofstream gpioexport(export_path.c_str());   //Open Export file.Convert C++ string to C string as required by all 										    	//linux pathnames
	if(gpioexport.fail()){
		std::cout<<"OPERATION FAILED : Unable to open GPIO "<<gpio_pin_num<<"."<<std::endl;
		return false;
	}

	gpioexport<<gpio_pin_num; //Write gpio number to export
	gpioexport.close(); //Close export file
	return true;

}

bool gpio::gpio_unexport()
{	std::string line;
	std::fstream config("config.txt");
	if(config.fail())
	{
		std::cout<<"cannot open file";
		return false;
	}
	config >> line;
	std::string unexport_path;
	unexport_path.append("/export");
	config << line;
	std::ofstream gpiounexport(unexport_path.c_str()); // OPens unexport file
	if(gpiounexport.fail()){
		std::cout<<"OPERTAION FAILED : unable to open unexport GPIO"<<gpio_pin_num<<"."<<std::endl;
		return false;
	}
	gpiounexport<<gpio_pin_num; //Write GPIO number to unexport
	gpiounexport.close(); //Close unexport file
	return true;
}

bool gpio::gpio_set_dir(std::string DIR)
{	//string pinstring=Integer.tostring(gpio_pin_num);
	std::string line;
	std::fstream config("config.txt");
	if(config.fail())
	{
		std::cout<<"cannot open file";
		return false;
	}
	config>>line;
	std::string setdir_path;
	setdir_path.append("/gpio");
	setdir_path.append(std::to_string(gpio_pin_num));
	setdir_path.append("/direction");
	std::ofstream gpiodir(setdir_path.c_str()); //Opens direction file for gpio
	if(gpiodir.fail()){
		std::cout<<"OPERATION FAILED: unable to set the direction of GPIO"<<gpio_pin_num<<"."<<std::endl;
		return false;
	}
	gpiodir<<DIR; //write direction to direction file
	gpiodir.close(); //close direction file
	return true;
}

bool gpio::gpio_set_value(int VAL)
{	std::string line;
	std::fstream config("config.txt");
	if(config.fail())
	{
		std::cout<<"cannot open file";
		return false;
	}
	config>>line;
	std::string setval_path;
	setval_path.append("/gpio");
	setval_path.append(std::to_string(gpio_pin_num));
	setval_path.append("/value");
	std::ofstream gpiosetval(setval_path.c_str()); //Open value file for gpio
	if(gpiosetval.fail()){
		std::cout<<"OPERATION FAILED: unable to set the value of GPIO"<<gpio_pin_num<<"."<<std::endl;
		return false;
	}
	gpiosetval<<VAL; //write value for value file
	gpiosetval.close();
	return true;
}

bool gpio::gpio_get_value()
{	std::string line;
	std::fstream config("config.txt");
	if(config.fail())
	{
		std::cout<<"cannot open file";
		return false;
	}
	config>>line;
	std::string getval_path;
	getval_path.append("/gpio");
	getval_path.append(std::to_string(gpio_pin_num));
	getval_path.append("/value");
	std::ifstream gpiogetval(getval_path.c_str()); //Open value file for gpio
	if(gpiogetval.fail()){
		std::cout<<"OPERATION FAILED: unable to set the value of GPIO"<<gpio_pin_num<<"."<<std::endl;
		return false;
	}
	std::string VAL;
	gpiogetval>>VAL; //Read GPIO value from value file
	if(VAL !="0")
		VAL="1";
	else
		VAL="0";
	std::cout<<VAL;
	gpiogetval.close();//Close the value file
	return true;
}

int gpio::gpio_get_num(){
	return gpio_pin_num;
}

























