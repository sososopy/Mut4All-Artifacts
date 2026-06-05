
typedef unsigned long long ul;
constexpr ul fib(ul n) { return n < 2 ? n : fib(n-2) + fib(n-1); }
static_assert(fib(93) == 12200160415121876738ul, "bad result");

