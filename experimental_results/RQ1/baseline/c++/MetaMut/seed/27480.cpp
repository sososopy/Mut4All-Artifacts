
template<typename T, typename... Args>
  concept baz = requires (T t, Args... args) { *t; };

template<typename T>
  requires baz<T>
void foo() { }

void bar()
{
  foo<int>();
}

