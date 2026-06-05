
template <int I>
struct derp
{
        constexpr inline static int i = I;

    template <int J>
    void f(derp<J> d) requires(d.i < i)
    {
        __builtin_printf("derp %zu < %zu\n", d.i, i);
        derp<I> d2;
        f(d2);
    }
};

int main()
{
    derp<9> d9;
    derp<8> d8;

    d9.f(d8);
}
