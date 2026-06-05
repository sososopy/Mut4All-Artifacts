
#include <array>

void foo()
{
	[](auto v) {
		std::array<int, 3> bar;
	}(42);
}

