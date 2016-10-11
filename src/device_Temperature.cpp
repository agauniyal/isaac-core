#include "device_Temperature.hpp"
#include <fstream>

using namespace isaac;

const std::string TempSensor::TEMPSEN_PATH = config::getTempSensor();


TempSensor::TempSensor(const unsigned int _p, const std::string _n, const std::string _id,
  const std::string _f, std::chrono::milliseconds::rep _d)
    : Device(_p, _n, _id), folderName(_f), delayTime(_d)
{
	std::string path;
	path.reserve(40);
	path.append(TEMPSEN_PATH).append("/").append(folderName).append("/w1_slave");

	std::ifstream readStream(path, std::ios::ate);
	if (readStream) {
		std::streampos size = readStream.tellg();
		char c;

		for (int i = 1; i <= size; ++i) {
			readStream.seekg(-i, std::ios::end);
			readStream.get(c);
			if (c == '=') break;
		}
		readStream >> currTemperature;
		logger->info("Temperature Sensor <{}> - pin <{}> constructed", getName(), getPowerPin());
	} else {
		logger->error("Temperature Sensor <{}> cannot be created", getName());
		throw std::runtime_error("Error: Temperature Sensor cannot be created");
	}
}


int TempSensor::getTemperature()
{
	if (m_getTemp.try_lock()) {
		std::string path;
		path.reserve(40);
		path.append(TEMPSEN_PATH).append(folderName).append("/w1_slave");

		std::ifstream readStream(path, std::ios::ate);
		if (!readStream) {
			return -460;
		}

		std::streampos size = readStream.tellg();
		char c;

		for (int i = 1; i <= size; ++i) {
			readStream.seekg(-i, std::ios::end);
			readStream.get(c);
			if (c == '=') break;
		}

		readStream >> currTemperature;
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
		m_getTemp.unlock();
	}

	return currTemperature;
}


json TempSensor::dumpInfo() const
{
	auto j          = Device::dumpInfo();
	j["delayTime"]  = delayTime;
	j["folderName"] = folderName;
	j["type"]       = dToInt(deviceType::TempSensor);
	return j;
}
