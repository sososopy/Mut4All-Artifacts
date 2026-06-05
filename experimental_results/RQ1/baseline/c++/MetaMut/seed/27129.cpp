
template <typename Pred, typename Val>
bool test(Pred p, Val v)
{
    return p(v);
}

bool func(int* p)
{
    static constexpr auto is_zero = [](auto v) { return v == 0;};
    return test([](auto v){return is_zero(*v);}, p);
}

