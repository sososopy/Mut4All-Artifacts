

class Foo
{
  int a;
};

void f (const Foo &, int);

void g (auto)
{
  f({});
}

