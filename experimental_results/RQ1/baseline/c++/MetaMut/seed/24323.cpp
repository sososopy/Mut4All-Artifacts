
struct A
{
  struct B
  {
    int aa;
    int bb;
  };
  B cc;
};

void Fn1 (const A &);

void
Fn2 ()
{
  Fn1 ( { { bb: 0 } });
}
