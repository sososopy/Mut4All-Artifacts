
#include <iostream>

template<typename T>
class C
{
    template<typename U> // ????????
    friend class C;

    template<typename U>
    friend bool operator == (C const& c1, C<U> const& c2)
    {
      return c1.i == c2.i;
    }

    int i;

  public:

    C(): i {0} {}
    C(int val): i {val} {}
}; // template class C

int main()
{
  C<int>  c1 {5};
  C<char> c2 {5};

  if (c1 == c2) std::cout << "All ok" << std::endl;
  else          std::cout << "Very bad" << std::endl;



  return 0;
}

