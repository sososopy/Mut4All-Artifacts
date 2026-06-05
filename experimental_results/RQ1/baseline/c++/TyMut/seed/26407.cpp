
#include <initializer_list>

template <int S>
struct Array {

};

#ifdef USE_TEMPLATE
template <int N>
#endif
struct Foo
{
  static constexpr std::initializer_list<int> num = { 1, 2 };

  Array<num.size()> ctx;

};

#ifdef USE_TEMPLATE
void f(Foo<5>) { }
#else
void f(Foo) { }
#endif

