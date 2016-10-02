#include "device_temp.hpp" 
using namespace isaac;
const std::string Temp::TEMPSEN_PATH
  = config::getTempSensor();  // get path=/sys/bus/w1/devices
char c;

int Temp::getTemperature()
{
	if(flag!=true){
	flag = true;


	std::string path;
	path.reserve(40);
	path.append(TEMPSEN_PATH).append("/").append(folderName).append("/w1_slave");

	std::ifstream readStream(path);
	if (readStream.fail()) return -460;

	std::streampos size = readStream.tellg();
	for (int i = 1; i <= size; ++i)
		{
			readStream.seekg(-i, std::ios::end);
			readStream.get(c);
			if (c == '=') break;
		}

		readStream>>tempdata;
	std::this_thread::sleep_for (std::chrono::milliseconds(delayTime));
	flag=false;}
	return tempdata;
	}
