
namespace std
{
  template <class T1, class T2>
  struct pair
  {
    T1 first;
    T2 second;
  };
  template <class T>
  struct initializer_list
  {
  };
  template <typename T1, typename T2>
  struct map
  {
    map () {}
    map (initializer_list <pair <T1, T2>> l) {}
  };
}
template <class T>
void
foo (T &p)
{
  p.v = { { 0, { .a = 0, .b = 1 } } };
}
int
main ()
{
  struct { struct A { int a, b; }; std::map <int, A> v; } p;
  foo (p);
}

