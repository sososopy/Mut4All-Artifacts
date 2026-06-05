

#include <iostream>
#include <functional>

int main() {
    constexpr int t[2] = { 1, 2 };
	std::function<void()> foo = [&] {
		for (int i = 0; i < 2; ++i)
			std::cout << t[i] << ' ';
	};

	foo();
	return 0;
}

