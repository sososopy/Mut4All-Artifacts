template <auto v>
struct constant{};

template <auto x>
auto operator *(constant<x>) 
{ return constant<(*x)>{}; }
