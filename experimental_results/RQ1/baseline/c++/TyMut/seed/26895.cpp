
#include "atomic"

struct S
{
    explicit S (bool b = true)
    : m_counter{ b ? 1u : 0u }
    {}

    std::atomic<unsigned short> m_counter;
};

S x;


