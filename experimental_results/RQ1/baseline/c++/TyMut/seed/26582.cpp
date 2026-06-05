
struct no_copy
{
    no_copy() = default;

    no_copy(const no_copy&) = delete;
    no_copy(no_copy&&);

    no_copy& operator=(const no_copy&) = delete;
    no_copy& operator=(no_copy&&);
};

struct foo
{
    operator no_copy() &
    {
        return no_copy();
    }

#ifndef WORKAROUND1
    operator no_copy&&() && = delete;
#endif
};

void test()
{
    foo f;
    no_copy nc;

#ifndef WORKAROUND2
    nc = f;
#else
    nc = f.operator bar();
#endif
}
