template <unsigned>
struct x
{
};

void f()
{
}

x<(unsigned)&f> a;
