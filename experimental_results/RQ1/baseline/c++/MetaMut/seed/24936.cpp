
class A
{
protected:
  static void f ();
};

class B : public A
{
};

class C : public A
{
};

class D : public C, public B
{
  void g ()
  {
    D::f ();
  }
};
