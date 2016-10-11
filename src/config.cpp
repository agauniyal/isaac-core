#include "config.hpp"
#include <fstream>
#include <json.hpp>
#include <sstream>
#include <stdexcept>

using namespace isaac;

std::string config::getGPIOBasePath()
{
	using json = nlohmann::json;

	std::ifstream config("config.json");
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config        = json::parse(buffer);
		static std::string path = json_config.at("gpio_path");
		return path;

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}

std::string config::getJsonDBPath()
{
	using json = nlohmann::json;

	std::ifstream config("config.json");
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config        = json::parse(buffer);
		static std::string path = json_config.at("db_path");
		return path;

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}

std::string config::getLogPath()
{
	using json = nlohmann::json;

	std::ifstream config("config.json");
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config        = json::parse(buffer);
		static std::string path = json_config.at("log_path");
		return path;

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}


std::string config::getTempSensor()
{
	using json = nlohmann::json;

	std::ifstream config("config.json");
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config        = json::parse(buffer);
		static std::string path = json_config.at("tempSensor_path");
		return path;

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}
