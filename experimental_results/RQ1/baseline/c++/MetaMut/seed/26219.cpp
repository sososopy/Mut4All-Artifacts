
template<typename T> struct foo;

template<> struct foo<void()&> {};
template<> struct foo<void()> {};

int main()
{}

