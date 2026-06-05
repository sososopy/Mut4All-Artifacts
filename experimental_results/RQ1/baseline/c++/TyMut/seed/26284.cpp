

struct A
{
  virtual bool foo ();
};
struct B : public A
{
  constexpr B () : A (&::n) {}
};
