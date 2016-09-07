#ifndef DEVICE_LIST_HPP
#define DEVICE_LIST_HPP

#include "device.hpp"
#include <unordered_map>

namespace deviceList {

// umap is a constant lookup container with device ID
// as key and pointer to actual device object as value
using umap = std::unordered_map<std::string, std::unique_ptr<Device>>;

// array of name and ID tuple
using arrNameID = std::tuple<std::string, std::string>;

void removeBad(const umap);
arrNameID getFailed(const umap);
arrNameID getBad(const umap);
arrNameID getMounted(const umap);
arrNameID getUnmounted(const umap);
}

#endif
