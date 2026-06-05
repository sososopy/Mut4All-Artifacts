The code sample is as follows:

#include <complex>

struct real128
{
    __float128 m_value;

    template <typename T>
    constexpr bool get(T &rop) const
    {
        using value_type = typename T::value_type;

        rop.real(static_cast<value_type>(m_value));
        rop.imag(static_cast<value_type>(0));

        return true;
    }
};

constexpr auto fappo(real128 x)
{
    std::complex<double> ret{1,2};
    x.get(ret);
    return ret;
}

constexpr auto flap = fappo(real128{4});

gcc rejects this code, but clang accepts it. mp++ is a library for multiprecision arithmetic. Its programmers disable several code lines, when gcc compiles this project. Please check:
https://github.com/bluescarni/mppp/commit/628651b9928e35dc09c0c2de3d2561abababb55f
---------------------------------------------------------

