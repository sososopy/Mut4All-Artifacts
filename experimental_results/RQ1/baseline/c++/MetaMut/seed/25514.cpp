template <typename LHS, typename RHS>
class Bad {
	const LHS& lhs;
public:
	Bad(const LHS& _lhs) : lhs(_lhs) {}

	template <typename LHS2, typename RHS2>
	friend auto operator / (const Bad<LHS2, RHS2>& X, const RHS& rhs) -> decltype(X / rhs) {
		return X.lhs + rhs;
	}
};

void test()
{
	int a,b;
	Bad<int, int> t(a);
	auto r = t / b;
}