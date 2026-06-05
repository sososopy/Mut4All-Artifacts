

struct nullopt_t {
    enum class Hidden { Token };
    explicit constexpr nullopt_t(Hidden) noexcept { }
};

struct in_place_t {
    explicit constexpr in_place_t() = default;
};

template<typename T>
struct optional
{
    template<typename U = T>
    optional(U&&) { }

    optional(in_place_t);

    optional(nullopt_t);
};

struct Widget {
    int i[1];
};

optional<Widget> r1( {{}} );

optional<Widget> r2{ {} };
