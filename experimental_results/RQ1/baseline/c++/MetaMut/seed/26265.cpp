
class Interface {
public:
  virtual int f() = 0;
};

class Concrete : public Interface {
public:
  virtual int f();
};

int
Concrete::f()
{
  return 2;
}

Concrete*
do_stuff()
{
  Concrete* c = new Concrete();
  return c;
}

int
call_f()
{
  Interface* i = do_stuff();
  return i->f();
}

