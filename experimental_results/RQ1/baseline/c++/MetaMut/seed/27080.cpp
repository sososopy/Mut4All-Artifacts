
#include <cstdint>
#include <iostream>

constexpr uint64_t Fib(uint32_t i)
{
    return (i < 2)? i: (Fib(i - 1) + Fib(i - 2));
}

template <uint32_t Z, uint32_t N> struct List
{
    const uint64_t val = Fib(Z - N);
    List<Z, N - 1> next;
};

template <uint32_t Z> struct List<Z, 0>
{
    const uint64_t value = Fib(Z - 0);
};

template <uint32_t Z> struct Array
{
    const List<Z - 1, Z - 1> data;
    const uint64_t *table = reinterpret_cast<const uint64_t*>(&data);
    constexpr uint32_t size() const { return Z; }
};

Array<20u> array;

int main()
{
    for (uint32_t i(0); i < array.size(); ++i)
        std::cout << array.table[i] << " ";
}
