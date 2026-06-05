

#include <array>
#include <utility>

template<typename T, typename... Types>
inline const std::array<T, sizeof...(Types)>
make_array(Types... args) {
	return std::array<T, sizeof...(Types)>{{args...}};
}

int main()
{
    const auto tmp = make_array<int>(1, 2, 3);
}
