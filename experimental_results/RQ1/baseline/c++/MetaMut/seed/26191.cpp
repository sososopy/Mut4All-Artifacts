
#include <functional>

typedef const std::function<void ()> Foo;
void Apply(Foo processor);

struct Bar : public Foo {};

void Fetch() {
  Apply(Bar());
}

