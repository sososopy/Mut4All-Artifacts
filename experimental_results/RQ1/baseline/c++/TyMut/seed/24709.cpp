
namespace std
{
    template<class T>
    struct initializer_list
    {
    };
}

struct X
{
    explicit X(std::initializer_list<int> x);
};

int test()
{
    X x  = {10, 10, 10};
}
