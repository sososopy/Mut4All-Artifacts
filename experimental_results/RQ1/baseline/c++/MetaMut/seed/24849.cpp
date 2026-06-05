
template <typename T>
struct wrapper
{
    operator T() const&;

    operator T&&() &&;
};

wrapper<int> createInt();

int test()
{
    return createInt();
}

