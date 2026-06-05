
#include <compare>

struct A { 
    auto operator <=>(const A&) const = default;
    bool operator <(const A&) const = default;
};
struct B { 
    auto operator <=>(const B&) const = default; 
};
struct C : A, B {};

template<typename T>
concept Cmp = requires(T u, T v) { u < v; };

//auto cmp = C{} < C{}; //this correctly fails due to ambiguity
static_assert( !Cmp<C> ); //but this evaluates wrongly in GCC

