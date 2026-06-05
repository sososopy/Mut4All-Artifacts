
struct FooBase
{
  constexpr void point_to_me(FooBase*& p) { p = this; }

  FooBase* m_p = nullptr;
};

struct Foo : FooBase {};

constexpr bool test1()
{
  FooBase fb;
  Foo* pf = new Foo;

//  fb.m_p = pf;             // this works
  pf->point_to_me(fb.m_p); // this doesn't

  delete static_cast<Foo*>(fb.m_p);
  return true;
}
