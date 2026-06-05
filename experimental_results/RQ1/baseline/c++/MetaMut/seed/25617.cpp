
struct A;

template<int> struct B
{
  static void foo(A) {}
};

void bar()
{
  B<0>::foo(A());
}
