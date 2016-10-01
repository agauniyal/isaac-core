#ifndef DEVICE_LIST_HPP
#define DEVICE_LIST_HPP

#include "device.hpp"
#include <unordered_map>
#include <utility>

namespace isaac {

// umap is a constant lookup container with device ID
// as key and pointer to actual device object as value
using umap = std::unordered_map<std::string, std::unique_ptr<Device>>;

// array of name and ID tuple
using arrIdName = std::vector<std::pair<std::string, std::string>>;

class deviceList final {
	umap list;
	std::mutex m_list;

	static std::string genId(const unsigned int = 8);

	static const std::shared_ptr<spdlog::logger> logger;

	static const std::string JSONDB_PATH;

	deviceList(const deviceList &) = delete;
	deviceList &operator=(const deviceList &) = delete;

public:
	deviceList() {}
	arrIdName getAll() const;

	bool place(deviceType, const json = json::object());
	bool removeId(const std::string);
	void sync(const bool = 1, const std::string = JSONDB_PATH + "db.json");

	auto size() const { return list.size(); }
	auto empty() const { return list.empty(); }
	auto max_size() const { return list.max_size(); }
	auto clear() { return list.clear(); }
	auto count(const std::string _key) { return list.count(_key); }
	auto rehash(const size_t n) { return list.rehash(n); }
	auto reserve(const size_t n) { return list.reserve(n); }

	bool mt_place(deviceType _Type, const json _j)
	{
		std::lock_guard<std::mutex> lock(m_list);
		return place(_Type, _j);
	}

	bool mt_removeId(const std::string _id)
	{
		std::lock_guard<std::mutex> lock(m_list);
		return removeId(_id);
	}

	void mt_sync(const umap &)
	{
		std::lock_guard<std::mutex> lock(m_list);
		sync();
	}
};
}

#endif
