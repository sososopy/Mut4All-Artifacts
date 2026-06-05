
constexpr int foo (int n)
{
    int a [n] = { 0 };

    int z = a [0] + (n ? foo (n - 1) : 0);

    return z;
}

constexpr int i = foo (3);
