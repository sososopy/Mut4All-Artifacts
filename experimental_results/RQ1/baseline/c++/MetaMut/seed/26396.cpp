
struct A {
   A(int,int);
};

void
g (int a)
{
  A b2(auto(a), 42);
}

