int main
{
    static const int x = 5;
    const int * const y = &x;
    static_assert(y, "");
}
