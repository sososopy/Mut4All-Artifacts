
#include <array>

template <int N>
using A = std::array<int, N>;

// Ensures a specialization of A is deduced, from https://eel.is/c++draft/over.match.class.deduct#6
template <class> class AA;
template <int N> class AA<A<N>> {};
template <class T> concept deduces_A = requires { sizeof(AA<T>);} ;

static_assert(deduces_A<std::array<int, 3>>); // fails on gcc, true on clang and msvc
