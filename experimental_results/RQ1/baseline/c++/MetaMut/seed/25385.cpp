
template <typename T>
struct A {};
struct B
{
  static B & foo () {}
  template <typename T> A <T> bar (unsigned) {}
};
template <typename T> struct C {};
template <typename T> struct D
{
  typedef C <T> U;
  explicit D (A <int> x = B::foo ().bar<int> (sizeof (U))) {}
};
struct E
{
  void baz ();
  D <int> e[1];
};
struct F
{
  bool baz ();
};
bool F::baz ()
{
  E ().baz ();
}
