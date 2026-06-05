
struct Struct {
    template <typename T, typename = void>
    constexpr static bool use_cond = false;
    template <typename T>
    constexpr static bool use_cond<T, void> = true;
};

