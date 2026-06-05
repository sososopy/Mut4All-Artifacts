
#include <chrono>
#include <string>

bool bug(std::chrono::milliseconds lhs, std::chrono::milliseconds rhs) {
	return lhs < rhs; // spurious "warning: zero as null pointer constant"
}

bool bug(std::string::const_iterator lhs, std::string::const_iterator rhs) {
	return lhs < rhs; // spurious "warning: zero as null pointer constant"
}
