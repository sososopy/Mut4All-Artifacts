
template <typename A, void (A::*F)()>
struct T {};

struct foo
{
    void bar() const {}
    typedef T<foo const, &foo::bar> bar_type;
};

int main()
{
    return 0;
}


