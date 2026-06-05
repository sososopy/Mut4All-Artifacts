int g(int a) [[pre: (f(a) > a)]]
{
    int r = a - f(a);
    return 2 * r;
}
