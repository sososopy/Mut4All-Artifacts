
inline namespace N __attribute ((__abi_tag__ ("cxx11")))
{
  struct A {};
}
template <typename T>
struct B { typedef int size_type; };
struct S1 { B<A>::size_type foo () const { return 1; } };
struct S2 { B<A>::size_type foo () const; };
int S2::foo () const { return 2; }

int
main ()
{
  auto f1 = &S1::foo;
  auto f2 = &S2::foo;
}
