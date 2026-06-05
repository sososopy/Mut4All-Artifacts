
template <template <class> class> class __meval{};
template <class _Fun, class>
  using __tuple_types_fn = decltype(_Fun(), [] {});
template <class _Fun>
struct __mexpand {
  template <class _MetaFn> using __f = __tuple_types_fn<_Fun, _MetaFn>;
};
auto t = __meval<__mexpand<int>::template __f>();

