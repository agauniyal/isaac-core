#ifndef DEVICE_TEMP_HPP
#define DEVICE_TEMP_HPP

#include "device.hpp"
#include <chrono>
#include <fstream>
namespace isaac {
class Temp final : public Device {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;
	std::string folderName;
	Temp(const Temp &) = delete;
	Temp &operator=(const Temp &) = delete;
	static const std::string TEMPSEN_PATH;  // done
	std::string sensorFileName;  // done
	bool flag;  // done
	std::chrono::milliseconds::rep delayTime;  // done
	int tempdata;  // done

public:
	Temp(const unsigned int _p, const std::string _n = "", const std::string _id = "",
	  const std::string folder = "", std::chrono::milliseconds::rep _z = 2000)
	    : Device(_p, _n, _id), delayTime(_z), folderName(folder)
	{
		logger->info("Led <{}> - pin <{}> constructed", getName(), getPowerPin());
		char c;
		std::string path;
		path.reserve(40);
		path.append(TEMPSEN_PATH).append("/").append(folder).append("/w1_slave");

		std::ifstream readStream(path);
		readStream.seekg(1, std::ios::end);
		std::streampos size = readStream.tellg();
		for (int i = 1; i <= size; ++i) {
			readStream.seekg(-i, std::ios::end);
			readStream.get(c);
			if (c == '=') break;
		}
		readStream >> tempdata;
	}

	Temp(const json _j, const std::string _id = "") : Temp(_j["powerPin"], _j["name"], _id) {}
	int getTemperature();  // done

	deviceType getType() const override { return deviceType::TempSensor; }  // done

	std::chrono::milliseconds::rep getDelay() const { return delayTime; }  // done
};
}

#endif
