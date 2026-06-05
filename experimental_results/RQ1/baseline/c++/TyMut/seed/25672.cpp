
#include <new>

void * f(int i)
{
  return new int[i];
}

int main()
{
      f(-1);
}

