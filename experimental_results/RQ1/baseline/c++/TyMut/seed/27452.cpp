
template<auto = []{}>
constexpr auto nested() {
	return 1;
}

template<typename T>
constexpr void test() {
	static_assert(nested());
}

