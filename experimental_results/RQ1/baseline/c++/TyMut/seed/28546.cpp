
template <typename T>
concept test = requires
{
    new T[1]{{ 42 }};
};

struct foobar
{
    foobar(int);
};

int main()
{
    static_assert(test<foobar>);
    new foobar[1]{{ 42 }};
}
