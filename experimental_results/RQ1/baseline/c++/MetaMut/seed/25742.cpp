
struct Private
{
    Private(const Private&) = delete;
    Private() = default;
};

struct A
{
    static constexpr Private p = Private();
};
