
template <typename>
void f ()
{
  struct S { struct T typedef a; };
  struct S::a b;
}
void g ()
{
  f<int> ();
}

