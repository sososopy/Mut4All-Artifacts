
template <typename T>
struct A {
	const T s_;
	A() = default;
	A(const T& s) : s_(s) {};
};


int main() {
	A<bool> b(true);
};

