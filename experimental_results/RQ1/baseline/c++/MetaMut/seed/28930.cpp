
constexpr bool f() {
	auto ptr = new int();
	delete ptr;
	return true;
}

static_assert(f());
