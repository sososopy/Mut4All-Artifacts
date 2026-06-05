struct S {
};

struct T {
	static int m;
};

template<typename T>
auto a(T x) {
	int * y = x;
}
void b() {
	a(S::m);
}