
#include <iostream>

class A
{
public:
   int a;
   virtual ~A(){}
};

class B : public A
{
public:
   int b;
};

void test(A* a)
{
   B& rrr = *static_cast<B*>(a);
   std::cout << "test\n";
}

int main()
{
   B bb;
   test(&bb);
   return 0;
}
