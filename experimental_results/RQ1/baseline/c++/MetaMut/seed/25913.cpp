
extern "C" int printf(const char *,...);

struct A {
	A() { printf("A()\n"); }
	A(const A&) { printf("A(const A&)\n"); }
	~A() { printf("~A()\n"); }
	operator int() const;
};
struct B {
	B() { printf("B()\n"); }
	B(const B&) { printf("B(const B&)\n"); }
	~B() { printf("~B()\n"); }
	operator int() const;
};

int main() {
	A a;
	B b;
	try { auto lll ( [a,c = (throw 7, 9),b]()->int { return a+c+b; } ); // gcc doesn't destroy copy-captured a...
	} catch (...) {
	}
};
