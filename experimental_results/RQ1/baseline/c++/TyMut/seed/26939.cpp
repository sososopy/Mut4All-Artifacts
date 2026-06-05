

void test(int);

namespace N {

struct A
{
  friend void test(A); // line 7
};

struct B
{
  friend void test(B); // line 12
};

void f()
{
  //using ::test;  // line 17
  test(0); // line 18
}
  
}
