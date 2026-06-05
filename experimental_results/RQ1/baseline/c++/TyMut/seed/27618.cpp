
struct B
{
    template <class U>
    friend void f(B)
    {
    }
};

void g()
{
    f<int>(B{});
}
