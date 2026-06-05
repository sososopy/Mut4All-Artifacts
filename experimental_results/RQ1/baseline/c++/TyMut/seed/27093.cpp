template<class T>
struct A {
    struct B{};
};

template<> struct A<char>::B{
    void f();
    struct C;
    struct D{}; // ok
};

void A<char>::B::f(){}// ok 

struct A<char>::B::C{};// error

int main()
{
    return 0;
}

