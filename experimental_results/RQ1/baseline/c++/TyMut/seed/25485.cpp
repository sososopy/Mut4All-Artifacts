
#include <memory>
struct foo {};

int main ()
{
  std::shared_ptr<foo> a = 0;
  std::shared_ptr<foo> b(0);
  std::shared_ptr<foo> c{0};
  foo *d = 0;
}
