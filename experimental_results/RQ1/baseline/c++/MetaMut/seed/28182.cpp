
enum class foo
{
    bar,
    baz,
};

int func(foo f)
{
    switch (f)
    {
    case foo::bar:
        return 5;
    case foo::baz:
        return 10;
    }
}
