
template<int N>    struct S {};
template<S s>      struct U {};
template<typename> struct V { V() = delete; };

template <int N, S<N> s>
struct V<U<s>> {};

V<U<S<0>{}>> v{};  // #1

int main() {}
