
template <int X>
struct Base {
	void func(int);
};

struct Derived : Base<0> { };

template <int X>
void Base<X>::func([[maybe_unused]] int n) { }  // WARNING

template class Base<0>;

