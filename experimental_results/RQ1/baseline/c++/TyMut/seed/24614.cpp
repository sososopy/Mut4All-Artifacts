struct Foo
{
protected:
  Foo() = default;
  int t;
};

struct Bar : public Foo
{
  Bar(): Foo{} { }
};


