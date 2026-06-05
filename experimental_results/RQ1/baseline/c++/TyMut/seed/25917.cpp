
#include <cstddef>
#include <new>

template <typename T = size_t>
void f ()
{
   size_t coord [2][2];
   new (&coord) size_t [2][2] 
   {
      {0,0},
      {0,0},
   };
}

int main ()
{
   f<>();
}
