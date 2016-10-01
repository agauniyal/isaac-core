#include "deviceList.hpp"
#include "device_Led.hpp"
#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>

using namespace isaac;

const std::string deviceList::JSONDB_PATH = config::getJsonDBPath();
const std::shared_ptr<spdlog::logger> deviceList::logger
  = spdlog::rotating_logger_mt("DL_Logger", config::getLogPath() + "deviceList", 1048576 * 5, 3);


std::string deviceList::genId(const unsigned int _len)
{
	std::string result;
	result.reserve(_len);

	static constexpr char alphanum[] = "0123456789"
	                                   "abcdefghijklmnopqrstuvwxyz"
	                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	const auto seed = std::random_device()();
	static std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis(0, 61);

	for (unsigned int i = 0; i < _len; i++) {
		result += alphanum[dis(gen)];
	}
	return result;
}


bool deviceList::place(deviceType _Type, const json _j)
{
	std::string id;
	id.reserve(8);

	do {
		id = genId(8);
	} while (list.count(id) != 0);

	switch (_Type) {

		case deviceType::Led: {
			try {
				auto res = list.emplace(std::make_pair(id, std::make_unique<Led>(_j, id)));
				logger->info("Device inserted\n{}", _j.dump(4));
				return res.second;
			} catch (const std::exception &e) {
				logger->info(e.what());
				return false;
			}
		}
		default: return false;
	}
}


arrIdName deviceList::getAll() const
{
	arrIdName result;
	for (auto &el : list) {
		result.emplace_back(el.second->getId(), el.second->getName());
	}
	return result;
}


bool deviceList::removeId(const std::string _id)
{
	auto device = list.find(_id);
	if (device != list.end()) {
		device = list.erase(device);
		logger->info("Device with id <{}> removed", _id);
		return true;
	}
	logger->info("Device with id <{}> doesn't exist, not removed", _id);
	return false;
}


void deviceList::sync(const bool memToDisk, const std::string _f)
{
	if (memToDisk) {
		std::ofstream db(_f.c_str(), std::ofstream::trunc);
		if (db) {
			json devices = json::array();
			for (auto &el : list) {
				auto device = el.second->dumpInfo();
				devices.push_back(device);
			}
			db << devices.dump(4) << std::endl;
		} else {
			throw std::runtime_error("could not open db for sync");
		}
	} else {
		std::ifstream db(_f.c_str());
		if (db) {
			std::stringstream buffer;
			buffer << db.rdbuf();
			auto devices = json::parse(buffer);

			for (auto &d : devices) {
				auto type   = intToD(d["type"]);
				auto result = place(type, d);
			}

		} else {
			throw std::runtime_error("could not open db for sync");
		}
	}
}
