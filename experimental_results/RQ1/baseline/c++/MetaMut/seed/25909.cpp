

template <int x, int y, int z>
void foo()
{
}

template <void (*f)()>
void adapter1()
{
    f();
}

template <void (*f)()>
void adapter2()
{
    f();
}

template <void (*f)()>
void adapter3()
{
    f();
}


void snafu()
{
    adapter1<foo<1234567,31415,271828>>();
    adapter2<foo<1234567,31415,271828>>();
    adapter3<foo<1234567,31415,271828>>();
}


template <int x, int y, int z>
void foo()
{
}

template <void (*f)()>
void adapter1()
{
    f();
}

template <void (*g)()>
void adapter2()
{
    adapter1<g>();
}


void snafu()
{
    adapter2<foo<1234567,31415,271828>>();
}

