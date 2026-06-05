
extern "C" int printf(const char *,...);
struct B {
	B(int,int) { printf("CB %p\n",this); }
	B(const B&) { printf("const CB %pn\n",this); }
	B(B&&) { printf("const CB %pn\n",this); }
	~B() { printf("B %p\n",this);  }
};
struct C {
	int c;
	int c2;
};
struct A {
	 struct B b;
	 struct C c;
};
A test() {
	//const A a1 = { { 1, 2 }, { 3, (throw 9, 4) } } ; // destructor for B called
	//const A &a2 = { { 1, 2 }, { 3, (throw 9, 4) } } ; // destructor for B not called
	return { { 1, 2 }, { 3, (throw 9, 4) } } ; // destructor for B not called
};
int main() {
	try {
		test();
	} catch (...) {
	}
}
