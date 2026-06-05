
template<class T> constexpr void test_constexpr_helper(T&&) {}
constexpr bool test_fun(bool expr) {
	return noexcept(test_constexpr_helper(expr));
}
static_assert(test_fun(true), "inside static_assert it's always constexpr");
int main(){}
