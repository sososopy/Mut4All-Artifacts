struct A { A () : a (true) {} bool a; ~A () {} };
struct B { };
struct C : B { C () {} ~C () {} };
struct D { D () : d (true) {} bool d; ~D () {} };
struct E : public A, C, D { E () : e (true) {} bool e; ~E () {} };

int
main ()
{
  E e;
  if (!e.a || !e.d || !e.e)
    __builtin_abort ();
}

