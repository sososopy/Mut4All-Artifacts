
#include <iostream>

enum class foo :int {x,y,z};

    //class we're trying to generate
template <int a, foo b>
class A
{
public:
  A()
  {
    std::cout << a << "," << (int)b << std::endl;

  }
};

//class which generates information
template <typename T>
struct B
{
  typedef T value_type;
  static const T val;
};

template <typename... B>
struct madscience_intitializer
{
  template <typename B::value_type... args>
  using ret_type = A<args...>;
};




int main(int argc, char** argv)
{
  madscience_intitializer<B<int>,B<foo> >::ret_type<1,foo::y> a;
}

