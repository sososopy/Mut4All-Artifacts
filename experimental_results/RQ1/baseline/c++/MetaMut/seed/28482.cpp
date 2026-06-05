
template <typename R, typename P>
constexpr auto none_of(R&& r, P p) -> bool {
    for (int i : r) {
        if consteval {
            if (p(i)) { // <== line 5
                return false;
            }
        } else {
            if (p(i)) { // <== line 9
                return false;
            }
        }
    }
    return true;
}

constexpr int vals[] = {1, 0, -1};
static_assert(none_of(vals,
    [](int i) consteval {
        return i > 2;
    }));

