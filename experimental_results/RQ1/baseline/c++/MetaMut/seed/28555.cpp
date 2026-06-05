

template<Size N, typename T>
constexpr decltype(auto) get(T tuple) { return *tuple(Get<N>{}); }

