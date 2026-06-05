struct A
{
    constexpr explicit A(bool b) : o{flip(b)} { }

    constexpr bool flip(bool b) { return !b; }

    bool o;
};
