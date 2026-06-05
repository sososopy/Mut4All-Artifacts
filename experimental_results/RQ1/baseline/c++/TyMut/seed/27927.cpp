int *ptr;

const int *const &
f()
{
  return ptr;
}

