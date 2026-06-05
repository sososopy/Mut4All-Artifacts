
template <int lshift>
struct DoesntWarn {
    static constexpr unsigned int v = lshift < 32 ? 1U << lshift : 0;
};

static_assert(DoesntWarn<32>::v == 0, "Impossible occurred");

template <int lshift>
struct SpuriouslyWarns {
    static constexpr bool okay = lshift < 32;
    static constexpr unsigned int v = okay ? 1U << lshift : 0;
};

static_assert(SpuriouslyWarns<32>::v == 0, "Impossible occurred");
