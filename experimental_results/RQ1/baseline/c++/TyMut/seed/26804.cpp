

class A
{
  void foo ()
  {
    [=] { return foo; };
  }
};
