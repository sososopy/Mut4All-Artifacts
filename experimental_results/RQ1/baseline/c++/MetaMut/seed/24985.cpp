
constexpr int foo (int n)
{
    int a [n] = { };

    int z = 0;

    for (unsigned i = 0; i != sizeof (a); ++i)
        z += a [i];

    return z;
}

constexpr int n = foo (1);
