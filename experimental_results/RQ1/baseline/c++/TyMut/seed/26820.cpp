
struct A {
    int n;
    int a [];
};

struct A foo (void)
{
    static struct A a = { 2, { 1, 0 } };

    return a;
}

