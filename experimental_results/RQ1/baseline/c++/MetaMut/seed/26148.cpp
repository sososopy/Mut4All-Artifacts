
template<int&...> struct A
{
  template<typename> struct B;

  template<typename T> struct B<T*> {};
};
