#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace gpio {

constexpr unsigned int NumPins = 40;
std::string getGPIOBasePath(const std::string);
}

#endif
