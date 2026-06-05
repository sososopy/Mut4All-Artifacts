
  //# 1 "foo.h" 3
  struct A
  {
    virtual void f();
  };
  
  //# 1 "bar.h"
  struct B : A
  {
    void f(int);
  };
