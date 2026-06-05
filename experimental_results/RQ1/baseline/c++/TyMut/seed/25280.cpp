
constexpr int a[] = { 1, 2, 3 };

constexpr int f (int i)
{
    int j = i + 1;
    return a[j];
}

constexpr int g (int i)
{
    int j = i + 1;
    return f (j);
}

constexpr int i = g (0);
constexpr int x = g (i);
