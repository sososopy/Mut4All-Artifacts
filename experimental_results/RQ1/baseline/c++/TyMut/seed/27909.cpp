
template <class T>
struct S
{
    template <class U, class V>
    static void foo(V)
    {
    }
    
    void bar()
    {
        foo<void>(1);
    }
};
