

#include <iostream>

template <typename R>
  concept bool Concept()
  {
    return requires( R r ) {
      std::begin(r);
      std::end(r);
      requires __is_same_as( 
        decltype(std::begin(r)), decltype(std::end(r)) );
    };
  }

struct A
{
  A() = default;
  A( const A& ) = default;

  template <Concept R>
    explicit A( R&& r )
    {}
};

struct C : A
{
  C() = default;
  C( const C& ) = default;
};

int main()
{
  C c;
  return 0;
}

