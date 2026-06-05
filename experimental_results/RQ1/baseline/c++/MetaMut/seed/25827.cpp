template <template <typename> class... Templates>
struct template_tuple {};
template <typename T>
struct identity {};
template <template <typename> class... Templates>
template_tuple<Templates...> f7() {}

void foo() {
  f7<identity>();
}
