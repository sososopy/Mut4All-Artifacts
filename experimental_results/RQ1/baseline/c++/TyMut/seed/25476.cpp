
#include <stdlib.h>
#include <new>
 
struct X {
  enum Type { Int, Double };
  Type type;
  char data[1];
};
 
int main()
{
  X* p = (X*)malloc(sizeof(X) + sizeof(double) -1);
  double* d = new (p->data) double(1.0);
  p->type = X::Double;
}

