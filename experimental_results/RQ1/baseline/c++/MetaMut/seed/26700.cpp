
#include <complex>

struct plus {};
struct multiplies {};
// ...

template<typename T, typename Op>
constexpr T identity_element;

template<>
constexpr int identity_element<int, plus> = 0;

template<typename T>
constexpr std::complex<T> identity_element<std::complex<T>, plus> = {
    identity_element<T, plus>,
    identity_element<T, plus>
};


