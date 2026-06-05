
#include <tuple>

struct A { int x; };
 
namespace std
{
    template<>
    struct tuple_size<::A> {};
}
 
auto [x] = A{};

