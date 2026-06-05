
template<typename T>
concept A = true;

template<typename T>
concept B = A<T> and true;

constexpr bool f(A auto) {
	return false;
}

constexpr bool f(B auto const &) {
	return true;
}

static_assert(f(0));
