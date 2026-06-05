
struct A {
    template<int i>
    inline int
    a()
    {
        return 0;
    }
    template<int i>
    inline auto
    b() -> decltype(a<i>())
    {
        return a<i>();
    }
};

int
main()
{
    A a;
    a.b<2>();
}
