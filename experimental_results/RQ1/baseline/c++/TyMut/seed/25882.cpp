
template<typename> void foo(const auto&) {}

template<typename, typename...T> void foo(const auto&, T...) {}
