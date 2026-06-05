

struct A
{
  int x;

  void foo () const {
    (A::x);
  };
};

void
foo ()
{
  A ().foo ();
}
