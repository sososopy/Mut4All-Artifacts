

struct A { };

A operator+ (A &) { return A (); }
A operator+ (const A &) { return A (); }


template <typename T>
void
foo ()
{
  +A ();
}

void
bar ()
{
  foo<int> ();
}

