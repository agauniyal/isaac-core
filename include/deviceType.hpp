#include <type_traits>

namespace isaac {
enum class deviceType { Base, Led };

template <typename E> constexpr auto dToInt(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}
constexpr deviceType intToD(int i) noexcept { return static_cast<deviceType>(i); }
}
