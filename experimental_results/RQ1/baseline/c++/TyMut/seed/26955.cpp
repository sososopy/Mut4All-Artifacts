
#include <iostream>
#include <stdint.h>

template <class T> void buggy_code_x64(T &dest) {			// THIS CODE IS EXECUTED CORRECTLY ONLY IN O0 OR O1 OPTIMIZATION MODES
	dest=0x08;							// L1
	for (size_t k=0; k<7; k++) dest=(dest*16);			// L2
	// Here dest=0x80000000, this number cannot be represented by a signed 32-bit integer value.
	if (dest<0) std::cout << "Negative number, dest=" << dest;
	else std::cout << "Non-negative number, dest=" << dest;
}


template <class T> void correct_code_x64(T &dest) {			// THIS CODE IS ALWAYS EXECUTED CORRECTLY.
	dest=0x0800;							// L1
	for (size_t k=0; k<5; k++) dest=(dest*16);			// L2
	// Here dest=0x80000000, this number cannot be represented by a signed 32-bit integer value.
	if (dest<0) std::cout << "Negative number, dest=" << dest;
	else std::cout << "Non-negative number, dest=" << dest;
}

int main() {
int32_t val1;
	std::cout << "int32 test:" << std::endl;
	buggy_code_x64<int32_t>(val1);
	std::cout << std::endl;
int32_t val2;
	std::cout << "int32 test:" << std::endl;
	correct_code_x64<int32_t>(val1);
	std::cout << std::endl;
}
