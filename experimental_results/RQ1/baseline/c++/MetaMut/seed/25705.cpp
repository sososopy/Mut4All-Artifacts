
struct A
{
  ~A();
};

int foo(A = {});

int i = foo();
