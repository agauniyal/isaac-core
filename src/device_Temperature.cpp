#include "device_Temperature.hpp"

using namespace isaac;
const std::string TempSensor::TEMPSEN_PATH = config::getTempSensor();


TempSensor::TempSensor(const unsigned int _p, const std::string _n, const std::string _id,
  const std::string folder, std::chrono::milliseconds::rep _d)
    : Device(_p, _n, _id), folderName(folder), delayTime(_d)
{
	std::string path;
	path.reserve(40);
	path.append(TEMPSEN_PATH).append("/").append(folder).append("/w1_slave");

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
		busy = false;
		logger->info("Temperature Sensor <{}> - pin <{}> constructed", getName(), getPowerPin());
	} else {
		logger->error("Temperature Sensor <{}> cannot be created", getName());
		throw std::runtime_error("Error: Temperature Sensor cannot be created");
	}
}


int TempSensor::getTemperature()
{
	{
		std::lock_guard<std::mutex> lock(m_getTemp);
		if (busy) {
			return currTemperature;
		} else {
			busy = true;
		}
	}

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
	busy = false;
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
