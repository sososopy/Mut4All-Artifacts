
	#include <iostream>
	#include <sstream>
	#include <string>
	
	template<typename T>
	std::ostringstream& appendToStream(std::ostringstream& stream, const T& value) {
		stream << value;
		return stream;
	}

	template<typename T, typename... Args>
	std::ostringstream& appendToStream(std::ostringstream& stream, const T& value, Args... args) {
		stream << value;
		return appendToStream(stream, args...);
	}

	template<typename... Args>
	std::string createMessage(Args... args) {
		std::ostringstream x;
		appendToStream(x, args...);
		return x.str();	
	}

	template<typename... Args>
	std::string foo(int, Args... args) {
		return std::string("foo with int: " + createMessage(args...));
	}

	template<typename... Args>
	std::string foo(Args... args) {
		return std::string("foo without int: " + createMessage(args...));
	}

	template<typename T, typename... Args>
	std::string bar(int, Args... args) {
		return std::string("bar with int: " + createMessage(args...));
	}

	template<typename T, typename... Args>
	std::string bar(Args... args) {
		return std::string("bar without int: " + createMessage(args...));
	}

	int main(int, char**) {
		int i(-1);
		std::cout << foo(i, "Hello ", "world") << std::endl;
		std::cout << foo("Hello ", "world") << std::endl;
		std::cout << bar<float>(i, "Hello ", "world") << std::endl;
		std::cout << bar<float>("Hello ", "world") << std::endl;
	}


