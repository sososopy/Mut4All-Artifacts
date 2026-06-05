
#include <memory>

struct foo: std::enable_shared_from_this<foo>
{
  ~foo() { }
};

int main()
{
  std::make_shared<foo>();
}
