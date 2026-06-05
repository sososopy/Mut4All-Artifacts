
class Test
{
public:
    [[nodiscard]] int Get() const { return 42; }
};

template<typename T>
auto UsedForSFINAE(const T& t) -> decltype(t.Get(), void())
{
}

template<typename T>
auto UsedAsReturnValue(const T& t) -> decltype(t.Get())
{
    return t.Get();
}

int main()
{
    Test t{};

    UsedForSFINAE(t);
    UsedAsReturnValue(t);
}

