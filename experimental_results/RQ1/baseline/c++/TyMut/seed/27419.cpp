
struct S {
	int * m_ptr;

	constexpr S():
		m_ptr(new int())
	{
	}
	constexpr S(S && other) noexcept:
		m_ptr(other.m_ptr)
	{
		other.m_ptr = nullptr;
	}
	constexpr ~S() noexcept {
		delete m_ptr;
	}
};

constexpr bool test(S v) {
	auto x = static_cast<S &&>(v);
	return true;
}

static_assert(test(S()));
