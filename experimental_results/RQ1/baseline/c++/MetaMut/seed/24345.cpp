
struct A
{
  A x;
  virtual ~A() {}
};

struct B : A
{
  B() : A() {}
};
