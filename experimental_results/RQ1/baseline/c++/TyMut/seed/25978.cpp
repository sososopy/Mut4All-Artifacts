
class Foo {
  struct Bar { };
  friend class F;
};

class F
#ifdef BUG
  : public Foo::Bar
#endif
{
  void Fn() { Foo::Bar b; }
};

