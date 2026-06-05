
#include <vector>

constexpr auto get_vector() {
	std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };
	return vec;
}

constexpr auto get_vector_size() {
	constexpr auto vec = get_vector();
	return vec.size();
}

int main() {
	constexpr auto vec_size = get_vector_size();
        return 0;
}

