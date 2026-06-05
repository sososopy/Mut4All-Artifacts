
struct A
{
    void g()
    {
    }

    void f() const
    {
        [this]() { g(); }();
    }
};
