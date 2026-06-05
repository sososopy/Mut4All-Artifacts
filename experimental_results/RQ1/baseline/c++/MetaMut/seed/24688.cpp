
struct Foo {
  inline virtual void func() = 0;
};

struct Bar : public Foo {
  void func() { }
};

int main()
{
  Foo *f = new Bar;
  f->func();
}

