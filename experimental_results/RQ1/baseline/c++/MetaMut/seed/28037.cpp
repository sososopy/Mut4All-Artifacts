
#include <iostream>

template<typename T>
class A {
public:
    A() {}

    template<typename U>
    A(const A<U>&) {}

    bool operator==(const A<T>&) { return true; }
};

int main(int argc, const char *argv[])
{
    A<const std::string> a;
    A<std::string> b;
    if (a == b) {}
    return 0;
}
