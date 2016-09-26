#include <type_traits>

namespace isaac {
enum class deviceType { Base, Led };
template <typename T> constexpr auto to_int(T t) -> typename std::underlying_type<T>::type
{
	return static_cast<typename std::underlying_type<T>::type>(t);
}
}
