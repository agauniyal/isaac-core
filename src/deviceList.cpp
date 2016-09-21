#include "deviceList.hpp"
#include "device_LED.hpp"
#include <algorithm>
#include <random>

using namespace isaac;
using deviceList::umap;
using deviceList::arrIdName;


std::string deviceList::genId(const unsigned int _len)
{
	std::string result;
	result.reserve(_len);

	static constexpr char alphanum[] = "0123456789"
	                                   "abcdefghijklmnopqrstuvwxyz"
	                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	static const auto seed = std::random_device()();
	static std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis(0, 61);

	for (unsigned int i = 0; i < _len; i++) {
		result += alphanum[dis(gen)];
	}
	return result;
}


bool deviceList::place(umap &_l, const unsigned int _p, const std::string _n)
{
	std::string id;
	id.reserve(8);

	do {
		id = genId(8);
	} while (_l.count(id) != 0);

	auto res = _l.emplace(std::make_pair(id, std::make_unique<Led>(_p, _n, id)));
	return res.second;
}


arrIdName deviceList::getAll(const umap &_list)
{
	arrIdName result;
	for (auto &el : _list) {
		result.emplace_back(el.second->getId(), el.second->getName());
	}
	return result;
}


arrIdName deviceList::getFailed(const umap &_list)
{
	arrIdName result;
	for (auto &el : _list) {
		auto failed = el.second->hasFailed();
		if (failed) {
			result.emplace_back(el.second->getId(), el.second->getName());
		}
	}
	return result;
}


arrIdName deviceList::getBad(const umap &_list)
{
	arrIdName result;
	for (auto &el : _list) {
		auto bad = el.second->isBad();
		if (bad) {
			result.emplace_back(el.second->getId(), el.second->getName());
		}
	}
	return result;
}


arrIdName deviceList::getMounted(const umap &_list)
{
	arrIdName result;
	for (auto &el : _list) {
		auto mounted = el.second->isMounted();
		if (mounted) {
			result.emplace_back(el.second->getId(), el.second->getName());
		}
	}
	return result;
}


arrIdName deviceList::getUnmounted(const umap &_list)
{
	arrIdName result;
	for (auto &el : _list) {
		auto mounted = el.second->isMounted();
		if (!mounted) {
			result.emplace_back(el.second->getId(), el.second->getName());
		}
	}
	return result;
}


void deviceList::removeBad(umap &_list)
{
	for (auto it = _list.begin(); it != _list.end();) {
		it = (it->second->isBad()) ? _list.erase(it) : ++it;
	}
}


bool deviceList::removeId(umap &_list, const std::string _id)
{
	auto device = _list.find(_id);
	if (device != _list.end()) {
		device = _list.erase(device);
		return true;
	}
	return false;
}
