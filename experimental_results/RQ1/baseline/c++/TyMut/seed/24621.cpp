
#include <array>

using Ar = std::array<unsigned long, 10>;

template<typename T>
constexpr T Apply(const T& in, T (*f)(const T&)) { return f(in); }

static constexpr Ar id(const Ar& line) { return line; }
static constexpr Ar ar1 = {{1}};
static constexpr Ar results1 = Apply<Ar>(ar1, &id);

