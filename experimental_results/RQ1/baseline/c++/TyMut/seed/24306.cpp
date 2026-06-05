
class Int {
public:
	constexpr Int(long long unsigned value)
		: m_value(value)
	{}
	const long long unsigned m_value;
};

constexpr Int operator "" _i(long long unsigned i) {
	return Int(i);
}

auto x = 1_i.m_value;

