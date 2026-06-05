
#include <string>
#include <cstdint>

struct test
{
	std::string a;
	std::uint8_t b[16];
	std::uint8_t c[16];
};

test function(test blah)
{
	blah = {};
	return blah;
}

int main(int argc, char *argv[])
{
	return 0;
}

