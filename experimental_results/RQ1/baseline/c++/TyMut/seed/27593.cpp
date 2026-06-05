
#include <new>

class Base
{
public:
  Base();
  int* a;
  char b;
};

class Der : public Base {};

void baz()
{
  Der der;
  ::new (static_cast<Base*>(&der)) Base;
}
