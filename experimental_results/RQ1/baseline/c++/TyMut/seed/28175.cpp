
void foo(char*);

inline void bar(int n)
{
    if (__builtin_constant_p(n))
    {
        char a[(int)(n == 2 ? -1 : 0)];
        foo(a);
    }
}

void baz()
{
    bar(2);
}
