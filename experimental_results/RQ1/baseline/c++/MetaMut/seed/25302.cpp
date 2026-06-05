
static constexpr double foo = 10000.0;

int main()
{
    auto foobar = [foo](){return (double)foo;};
    return 0;
}
