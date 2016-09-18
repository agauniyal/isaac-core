#ifndef DEVICE_LIST_HPP
#define DEVICE_LIST_HPP

#include "device.hpp"
#include <memory>
#include <unordered_map>
#include <utility>

namespace isaac {
namespace deviceList {

	// umap is a constant lookup container with device ID
	// as key and pointer to actual device object as value
	using umap = std::unordered_map<std::string, std::unique_ptr<Device>>;

	// array of name and ID tuple
	using arrNameId = std::vector<std::pair<std::string, std::string>>;

	arrNameId getAll(const umap);
	arrNameId getFailed(const umap);
	arrNameId getBad(const umap);
	arrNameId getMounted(const umap);
	arrNameId getUnmounted(const umap);

	void removeBad(umap);
	bool removeId(const std::string, umap);
}
}

#endif
