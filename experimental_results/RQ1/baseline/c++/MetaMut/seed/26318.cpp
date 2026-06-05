
struct A {
	const int value1;
	int& value2;
};

struct B : A {};

int main() {
	B b; // no error
//	A a; // error, as expected
}
