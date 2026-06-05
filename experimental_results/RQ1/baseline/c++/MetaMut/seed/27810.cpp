
struct A {
  A(A &);
};
struct B : A { };

A foo ();

int
main ()
{
  B{foo()};
}

