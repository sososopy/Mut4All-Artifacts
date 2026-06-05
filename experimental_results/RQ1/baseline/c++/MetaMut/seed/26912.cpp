
auto f1(auto x) { return x++; }
decltype(auto) f2(auto x) { return x++; }
auto f3(auto x) -> int { return x++; }

are apparently treated as if they were equivalent to

template<class T>
T f1(T x) { return x++; }
// etc.

rather than 

template<class T>
auto f1(T x) { return x++; }
// etc.

This is easily observable with

struct X {
    int operator++(int) const { return 0; }
};

f1(X());

