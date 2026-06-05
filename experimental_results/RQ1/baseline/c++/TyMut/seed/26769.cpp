
auto count = [](auto&&... xs)
{
    return sizeof...(xs);
};

struct count_struct
{
    template<typename... Ts>
    auto operator()(Ts&&... xs)
    {
        return sizeof...(xs);
    }
};

int main()
{
    count(1,2,3,4,5,6,7);
    count_struct{}(1,2,3,4,5,6,7);
}

