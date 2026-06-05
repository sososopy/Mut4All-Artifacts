
constexpr bool f() {
	auto a = new int;
	auto b = new int;
	if (a == b) {
	}
	delete b;
	delete a;
	return true;
}

static_assert(f());
