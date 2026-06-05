
struct B
{
  B() {}
};

struct C
{
  B b = B();
};

C c;
