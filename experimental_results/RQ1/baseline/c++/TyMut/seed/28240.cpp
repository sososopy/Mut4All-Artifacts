
struct bar
{
    int id;
};

struct foo
{
    int id;
};

inline const bar& h(const foo& t)
{
    return reinterpret_cast<const bar&>(t);
}

int g(foo t)
{
    const bar& test = h(foo{t});

    if(test.id == 5)
        return 7;

    return test.id;
}

int main()
{
    int a = g(foo{5});
    int b = g(foo{99});
    return a + b;
}
