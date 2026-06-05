
struct Local
{
  void f();
};

Local *l;
void (Local::*ptr)();
decltype((l->*ptr)) i;         // { dg-error "member function" }

