struct A
{
        int i;
};

void foo(A& a)
{
        auto l = [&a.i]() { };
}

void bar(A& a)
{
        auto& i = a.i;
        auto l = [&i]() { };
}

void baz()
{
        A a;
        auto l = [&a.i]() { };
}
