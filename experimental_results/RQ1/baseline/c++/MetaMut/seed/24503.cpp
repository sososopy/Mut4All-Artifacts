
template <typename F, typename A,
          typename = decltype(static_cast<void (&)(A &)>(F::operator())(A()))>
void test();

