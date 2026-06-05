
template<typename T>
void def()
{}

template<void (*foobar)() = def<int>>
void bar()
{
    foobar();
}

template<typename T>
void foo()
{
    bar();
}

int main()
{
    foo<int>();
}

