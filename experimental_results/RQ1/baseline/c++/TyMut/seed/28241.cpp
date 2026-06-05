
struct empty
{};

template <typename T>
struct tuple_holder
{
    [[no_unique_address]] T value;
};

struct tuple : tuple_holder<int>, tuple_holder<empty>
{};

constexpr auto make_tuple(int&& i, empty&& e)
{
    return tuple{i, e};
}

int main()
{
    auto tuple = make_tuple(1, empty{});
    return static_cast<const tuple_holder<int>&>(tuple).value;
}
