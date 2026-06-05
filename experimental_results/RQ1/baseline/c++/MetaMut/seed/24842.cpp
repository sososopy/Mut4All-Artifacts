#include <array>

template<std::size_t N>
struct Literal
{
    std::array<char, N> arr;
    constexpr Literal(char const(&pp)[N]): arr(""){} 
};
 
template<Literal> 
constexpr auto operator""_S()
{
    return 4;
}
int main() {
    auto i = "test"_S;   
    auto j =  "ch"_S;       
}