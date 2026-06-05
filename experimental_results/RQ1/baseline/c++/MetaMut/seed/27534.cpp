
struct MyClass
{
  virtual ~MyClass () {}
  virtual void Doit () {}
};

int
main ()
{
  MyClass *c = new MyClass;
  c->~MyClass ();
  c->Doit ();

  return 0;
}

