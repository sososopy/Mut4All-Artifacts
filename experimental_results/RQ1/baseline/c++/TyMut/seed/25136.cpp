

template < typename T > 
int foo (int x, T t)
{
  return __transaction_atomic noexcept (foo) (1);  // foo should be "t" 
}
