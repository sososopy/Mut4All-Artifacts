
struct moveonly {
    moveonly() = default;
    moveonly(moveonly&&) = default;
};

template<class T>
constexpr auto is_throwable(T t) -> decltype(throw t, true) {
    return true;
}
template<class T>
constexpr bool is_throwable(...) { return false; }

constexpr bool b = is_throwable<moveonly>(moveonly{});

