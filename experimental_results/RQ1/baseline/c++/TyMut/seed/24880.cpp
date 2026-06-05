
#include <tuple>

void f ();
void f (int);

void g ()
{
  std::tie (f) = 0;
}
