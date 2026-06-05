
template <typename T>
struct A {
    template<typename T2>
    void f() {
	typedef int T;
    }
};

void g() {
    A<int> a;
    a.f<int>();
}
