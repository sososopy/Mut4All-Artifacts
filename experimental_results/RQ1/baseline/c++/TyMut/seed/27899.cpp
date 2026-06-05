
#include <stdexcept>
#include <string>

struct Test final {
	std::string m_string;

	Test(std::string string)
	: m_string{ !string.empty() ? std::move(string)
	            : throw std::invalid_argument{"string cannot be empty"} }
	{}
};

auto
main() -> int
{
	auto test = Test{"2"};
	return 0;
}
