
#include <type_traits>
struct future_state_base {
protected:
    ~future_state_base() noexcept;
};
#ifdef foo
    static_assert(std::is_nothrow_move_constructible<future_state_base>::value,
        "future_state_base's move constructor must not throw");
#endif
template <typename... T>
struct future_state : public future_state_base {
    static_assert(std::is_nothrow_move_constructible<future_state_base>::value,
        "future_state_base's move constructor must not throw");
};
void f(future_state<int> x) {
}
