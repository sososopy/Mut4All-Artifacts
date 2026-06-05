
namespace std { template<typename T> struct tuple_size; }
struct A { int x, y, z; };
template<> struct std::tuple_size<A> { static const int value = 3; };
constexpr int a = std::tuple_size<A>::value;
