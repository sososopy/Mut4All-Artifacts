typedef unsigned char byte;

constexpr bool f(byte x, byte y) { return x * y == 100; }

template<byte x, byte y> struct count_value {
	static const int value = f(x, y) ? 1 : 0;
};

template<byte x, byte y = 255> struct count_bound {
	static const int value = count_value<x, y>::value + (y ? count_bound<x, y-1>::value : 0);
};

template<byte x = 255> struct count {
	static const int value = count_bound<x>::value + (x ? count<x-1>::value : 0);
};

static_assert(count<>::value == 9, "bad value");