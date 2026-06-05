

inline int foo()
{
  return 42;
}

#endif



module;

export module FooMod;

export using ::foo;




import FooMod;

int main()
{
  return foo();
}

