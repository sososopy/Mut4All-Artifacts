
template<typename T, typename U = typename T::type>
concept foo = true;

constexpr bool f(foo auto) {
	return false;
}

constexpr bool f(int) {
	return true;
}

static_assert(f(0));
