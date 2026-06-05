
#include <string_view>
import <iostream>;

void f(std::string_view sv) {
	std::cout << sv;
}

int main() {
	f("hello\n");
}


