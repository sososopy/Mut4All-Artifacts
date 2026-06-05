
namespace A {
  template<typename T> struct B { };
}

using A::B;
template class B<int>;


