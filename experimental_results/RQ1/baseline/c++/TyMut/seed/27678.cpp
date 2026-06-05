
bool
foo ()
{
  return typeid (int) == typeid (long);
}

struct A { A (); virtual ~A (); };
struct B : public A {};

static inline bool
bar (A *p)
{
  return typeid (*p) == typeid (A);
}

bool
baz ()
{
  A a;
  return bar (&a);
}

bool
qux ()
{
  B a;
  return bar (&a);
}
