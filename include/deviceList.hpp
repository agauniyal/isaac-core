#ifndef DEVICE_LIST_HPP
#define DEVICE_LIST_HPP

#include "device.hpp"
#include <unordered_map>
#include <utility>

namespace isaac {
namespace deviceList {

	// umap is a constant lookup container with device ID
	// as key and pointer to actual device object as value
	using umap = std::unordered_map<std::string, std::unique_ptr<Device>>;

	// array of name and ID tuple
	using arrIdName = std::vector<std::pair<std::string, std::string>>;

	bool place(umap&, const unsigned int, const std::string = "");

	arrIdName getAll(const umap&);
	arrIdName getFailed(const umap&);
	arrIdName getBad(const umap&);
	arrIdName getMounted(const umap&);
	arrIdName getUnmounted(const umap&);

	void removeBad(umap&);
	bool removeId(umap&, const std::string);

	std::string genId(const unsigned int = 8);
}
}

#endif
