

auto foo() -> void
{
    int unused = 0;
    unused++;
}

and

auto foo() -> void
{
    int x;
    int unused;
    for ( ; x < 100; x++ )  unused++;
}

