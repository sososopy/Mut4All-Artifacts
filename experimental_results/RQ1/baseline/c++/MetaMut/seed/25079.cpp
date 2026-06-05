
constexpr int foobar()
{
  int array[100] = {};
  int *ar = array;
  if (ar == nullptr) // Error...
  {
    return 0;
  }
  return 1;
}
static_assert(foobar(),"");

