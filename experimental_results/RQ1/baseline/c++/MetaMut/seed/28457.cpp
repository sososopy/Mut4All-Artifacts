
struct EstablishedTiming {
    char m_dmt_id{};
};

auto x = [](auto) { constexpr EstablishedTiming established_timings3_bytes[]{{}}; };
