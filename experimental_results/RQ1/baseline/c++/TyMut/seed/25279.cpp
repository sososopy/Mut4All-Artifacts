
template < int > struct A {};
template < int ... N > void foo (A < (... + N) >) {};

void bar ()  
{
  foo < 1, 2, 3 > ({});
}
