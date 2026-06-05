#include <type_traits>

template<typename T>
auto compiles(T && t) -> T && {
	static_assert(std::is_same<T, int>::value, "Incorrect type deduced for T.");
	return t;
}
static_assert(std::is_same<int &&, decltype(compiles(0))>::value, "shouldn't compile");

auto fails(int && t) -> int && {
	return t;
}
static_assert(std::is_same<int &&, decltype(fails(0))>::value, "doesn't compile");
