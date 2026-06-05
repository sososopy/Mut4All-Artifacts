
template <class T> T&& declval();

template <class T>
auto foo() -> decltype(declval<T&>().bar()) {}

class Base
{
  private:
    template <class T>
    friend auto foo() -> decltype(declval<T&>().bar());

    void bar(){}
};

class Derived : public Base {};

int main()
{
  foo<Derived>();
}
