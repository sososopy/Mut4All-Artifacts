
struct D
{
  virtual D& f();
};

void g()
{
  D d;
  d.f().f().f().f().f().f().f().f().f().f().f().f().f().f().f()
#if 1
    .f().f().f().f().f().f().f().f().f().f().f()
#endif
    ;
}
