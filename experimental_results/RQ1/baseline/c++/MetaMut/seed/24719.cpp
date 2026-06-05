
#include <string>

struct A{
	static thread_local std::string s;

	void f() {
		this->s = "segv";
	}

	void g() {
		A::s =    "no segv";
	}

	void h() {
		s =       "no segv";
	}
};

thread_local std::string A::s;

int main() {
	A{}.f();
	return 0;
}

