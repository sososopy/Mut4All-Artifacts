
struct foo {
	friend auto test(const foo &) { return 0; }
};

