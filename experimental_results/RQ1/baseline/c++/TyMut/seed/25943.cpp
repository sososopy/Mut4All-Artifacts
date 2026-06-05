
void bar() __attribute__((noreturn));

template<class T>
struct Foo
{};

template<>
struct Foo<void()>
{
    static void func()
    {}
};

void foobar()
{
  Foo<decltype(bar)>::func();
}
