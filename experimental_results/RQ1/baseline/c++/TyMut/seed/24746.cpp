
//#include <math.h>
extern "C" double fmin(double, double);

template <typename T> void f(T, char (*)[fmin(T{}, T{}) ? 1 : 2] = 0);
constexpr bool f(int) { return true; }

static_assert(f(0.));
