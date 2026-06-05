
class B
{
protected:
  struct S {};
  virtual void doIt(struct S& s) = 0;
};

class D : public B
{
public:
  using B::S;
  virtual void doIt(struct S& s);
};

void D::doIt(struct S& s)
{
  (void)s;
}

