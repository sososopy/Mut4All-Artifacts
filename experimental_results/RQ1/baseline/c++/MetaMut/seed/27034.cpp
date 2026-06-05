

struct A
{
  virtual void f ();
};

struct B : A
{
  virtual void f () final;
};
