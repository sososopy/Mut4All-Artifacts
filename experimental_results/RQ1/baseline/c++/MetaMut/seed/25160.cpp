

#include <iostream>
#include <string>
#include <vector>

struct Foo {
  void test();
  void thing(void* a1, std::string a2);
};

void Foo::test() {
    auto foo = "gfdgfd";
    auto l = [&](auto asdf) {
         thing(asdf, foo);
    };
    l(nullptr);
}

void Foo::thing(void* a1, std::string a2) {
}

int main()
{
}
