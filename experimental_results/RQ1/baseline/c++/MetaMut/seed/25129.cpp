
class A
{
  virtual void foo () { }
};
class B : public A, A
{
};
B b1, &b2 = b1;
A a = b2;
