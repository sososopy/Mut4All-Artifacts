
#include <cstdint>
#include <iostream>

int main() {
	char32_t null = U'\u0000';
	std::cerr << "null (char32_t) = " << null << '\n';
	std::cerr << "null (uint32_t) = " << uint32_t(null) << '\n';

	char32_t soh = U'\u0001';
	std::cerr << "soh (char32_t) = " << soh << '\n';
	std::cerr << "soh (uint32_t) = " << uint32_t(soh) << '\n';

	std::cerr << "char32_t null == soh = " << (U'\u0000' == U'\u0001') << '\n';

	char16_t null16 = u'\u0000';
	std::cerr << "null (char16_t) = " << null16 << '\n';
	std::cerr << "null (uint16_t) = " << uint16_t(null16) << '\n';

	char16_t soh16 = u'\u0001';
	std::cerr << "soh (char16_t) = " << soh16 << '\n';
	std::cerr << "soh (uint16_t) = " << uint16_t(soh16) << '\n';

	std::cerr << "char16_t null == soh = " << (u'\u0000' == u'\u0001') << '\n';
}
