
template <class...>
constexpr bool Test = true;

template <typename...Ts, bool = (Test<Ts&&...>)>
void f();

