
template<typename T> constexpr int foo(T) { return 0; };
template<> int foo(int) { return 1; }

