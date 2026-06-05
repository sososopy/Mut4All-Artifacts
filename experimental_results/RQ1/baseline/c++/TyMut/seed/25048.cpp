

struct A {  template < class > void m ();  };
struct B : A {};
struct C : A, B {};

void foo ()
{
  C c; 
  c.C::m < int > ();
}
