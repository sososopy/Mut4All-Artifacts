
#include <cstddef>

template<std::size_t>
class bitset;

template<std::size_t N>
constexpr bool operator==(const bitset<N>&, const bitset<N>&) noexcept;

template<std::size_t N>
class bitset
{
    friend constexpr bool operator== <>(const bitset<N>&, const bitset<N>&) noexcept;
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ <-- error from this piece
};

template<std::size_t N>
constexpr bool operator==(const bitset<N>&, const bitset<N>&) noexcept
{
    return true;
}

int main() {}

