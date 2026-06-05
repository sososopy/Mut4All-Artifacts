
#include <cstdint>

uint16_t byteswap(uint16_t const a){
	constexpr uint8_t b[] = {
		8 * (2 - 1)
	};

	auto const x =
		(0x00ffULL & a) << b[0] |
		(0xff00ULL & a) >> b[0]
	;

	return static_cast<uint16_t>(x);
}

