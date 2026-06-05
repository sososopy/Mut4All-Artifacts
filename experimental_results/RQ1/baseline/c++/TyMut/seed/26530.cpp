
void foo(int&&) {}

int main()
{
  using T = int&&;
  int x = 0;
  T{x}; // ok -> should be error
  foo(T{x}); // ok -> should be error
  //T r{x}; // nok -> is good
}

