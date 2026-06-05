
struct A
{
  virtual auto foo() {}
};

struct B : A
{
  auto foo();
};

B b;
