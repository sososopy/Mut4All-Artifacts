
#include <new>
#include <stdexcept>
int main()  
{
  try 
  {
    int negative = -1;
    new char[negative];
  } 
  catch(const std::bad_array_new_length &e) 
  {
  }
  return 0;
}

