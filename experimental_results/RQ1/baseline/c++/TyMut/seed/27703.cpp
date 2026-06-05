
consteval int foobar(int i)
{
    static_assert(i > 1);
    return i + 2;
}

int main()
{
    return foobar(3);
}

