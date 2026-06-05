
template <typename T>
struct A {
    A() { }
    A(const A &) { }
};


struct B
{
    B();

    template <typename... Args>
    auto g(Args&&... args)
    {
       return [=](){ return f(args...); };
    }

    void f(A<char>,const char*) { }
};


B::B()
{
    g(A<char>(),"");
}
