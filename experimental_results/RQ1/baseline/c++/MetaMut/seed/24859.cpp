struct S
{
    S(auto value)
        requires (requires () { [&] -> decltype(value) {}; })
    {}
};