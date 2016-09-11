#include "deviceList.hpp"
#include <algorithm>

using deviceList::umap;
using deviceList::arrNameId;


arrNameId deviceList::getAll(const umap _list)
{
	arrNameId result;
	for (auto it = _list.begin(); it != _list.end();) {
		result.emplace_back(it->second->getId(), it->second->getName());
	}
	return result;
}


arrNameId deviceList::getFailed(const umap _list)
{
	arrNameId result;
	for (auto it = _list.begin(); it != _list.end();) {
		auto failed = it->second->hasFailed();
		if (failed) {
			result.emplace_back(it->second->getId(), it->second->getName());
		}
	}
	return result;
}


arrNameId deviceList::getBad(const umap _list)
{
	arrNameId result;
	for (auto it = _list.begin(); it != _list.end();) {
		auto bad = it->second->isBad();
		if (bad) {
			result.emplace_back(it->second->getId(), it->second->getName());
		}
	}
	return result;
}


arrNameId deviceList::getMounted(const umap _list)
{
	arrNameId result;
	for (auto it = _list.begin(); it != _list.end();) {
		auto mounted = it->second->isMounted();
		if (mounted) {
			result.emplace_back(it->second->getId(), it->second->getName());
		}
	}
	return result;
}


arrNameId deviceList::getUnmounted(const umap _list)
{
	arrNameId result;
	for (auto it = _list.begin(); it != _list.end();) {
		auto mounted = it->second->isMounted();
		if (!mounted) {
			result.emplace_back(it->second->getId(), it->second->getName());
		}
	}
	return result;
}


void deviceList::removeBad(const umap _list)
{
	for (auto it = _list.begin(); it != _list.end();) {
		it = it->second->isBad() ? _list.erase(it) : ++it;
	}
}


bool deviceList::removeId(const std::string _id, umap _list)
{
	auto device = _list.find(_id);
	if (device != _list.end()) {
		device = _list.erase(device);
		return true;
	}
	return false;
}
