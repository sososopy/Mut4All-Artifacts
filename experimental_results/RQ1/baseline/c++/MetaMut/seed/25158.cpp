

template <int I> 
void f ()
{
  enum E { a = f };
}

int
main ()
{
  f <0> (); 
  return 0; 
}
