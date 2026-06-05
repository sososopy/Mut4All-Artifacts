
#include <cmath>
#include <iostream>
int main(int argc, char* argv[]) {
	if (argc!=2) {
		std::cout << "Usage: \"./isqrt [integer]\"" << std::endl;
		return -1;
	} else {
		int arg = atoi(argv[1]);
		int isqrt = std::sqrt<int>(arg);
		std::cout << isqrt << std::endl;
		return isqrt;
	}
}

