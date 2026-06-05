
#include <vector>

class A : public std::vector< int >
{
public:
  A(int x) : std::vector< int >(x) { }
};

int main(int, char **)
{
  for (A v : v)
  {
    
  }
  
  return 0;
}

