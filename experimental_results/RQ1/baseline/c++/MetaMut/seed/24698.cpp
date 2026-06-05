
#include <array>

template <typename>
class order_of {
	public: static const std::size_t value = 0;
};


template <typename T, std::size_t N>
class order_of<std::array<T, N>> {
	public: static const std::size_t value = order_of<T>::value + 1;
};


template <typename T, std::size_t N, typename std::enable_if<order_of<T>::value == 0>::type* = nullptr> using vec = std::array<T, N>;


template <typename T, std::size_t N>
void F(const vec<T, N> & a) {
	
}


int main() {

	int a;
	std::array<int, 1> b;
	std::array<std::array<float, 2>, 1> c;

	//F(a);
	//F(b);
	F(c);

	return 0;
}



