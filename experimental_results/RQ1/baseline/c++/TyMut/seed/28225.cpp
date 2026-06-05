
template <typename ...Args>
struct C
{
    void f()
    {
        (
            [&, this]<typename Arg>()
            {
            }.operator()<Args>(),
            ...
        );
    }
};

int main()
{
    C<int>{}.f();

    return 0;
}

