#include <iostream>

template<class T>
int bar ( const T& ) { return 0; }

struct Foo
{
  int bar;
};

template<class F>
void callBar ( F& foo )
{
  if( foo.bar<0 )
    std::cout<<foo.bar<<std::endl;
}

int main ()
{
  Foo foo;
  foo.bar = -1;
  callBar( foo );
  return 0;
}

