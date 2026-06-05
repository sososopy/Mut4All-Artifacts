
struct A
{
  ~A();
};

struct B
{
  B(...);
};

B b(0, A());
