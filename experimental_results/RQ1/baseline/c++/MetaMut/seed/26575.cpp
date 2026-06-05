
template <typename... Ts> struct tuple {};
template <int... Ts> struct seq {};
template <typename TT, typename... Ts, int... i>
inline auto sfinae_friendly_apply_impl(TT invocable, tuple<Ts...> t,  seq<i...>) noexcept
    -> decltype(invocable(get<i>(t)...)) {}
void run_all() {
  sfinae_friendly_apply_impl([](){}, tuple<>{}, seq<>{});
}
template<int i, typename T> int get(T);

