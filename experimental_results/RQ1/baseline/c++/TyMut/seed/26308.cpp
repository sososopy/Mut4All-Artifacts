
int foo()
{
  return __transaction_atomic noexcept(false) (false);
}
