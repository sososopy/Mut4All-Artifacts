
struct X {
	int x(int, int) { return 1; }

	template<int>
	int x(int) { return 2; }
};
