


struct A 
{
  void f () {}
};

typedef void (A::*Ptr) ();

template < Ptr > struct B {};

template < class T > 
struct C : public A
{
  void bar ()
  {
    B < &A::A > b; // should be: B < &A::f > b;
  } 
};

template class C < int >;
