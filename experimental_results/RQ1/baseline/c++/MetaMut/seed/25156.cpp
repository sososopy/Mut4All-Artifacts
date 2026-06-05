

struct A
{ 
  constexpr A (int); // should be e.g.: constexpr A (int) {};
  constexpr operator int () const { return 0; }
};

template < int > struct B {};

constexpr A a = 0;
B < a > b;
