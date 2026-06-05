
template<class Base> struct derived:Base{};

struct X final{};

void foo(derived<X>*){}

int main()
{
  foo(0);
}

