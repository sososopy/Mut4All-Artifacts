
struct base {
    constexpr virtual ~base() = default;
};

struct derived final : base {
    constexpr virtual ~derived() = default;
};

constexpr derived der;

