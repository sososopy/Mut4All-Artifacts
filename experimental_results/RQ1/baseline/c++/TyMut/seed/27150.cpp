
template <typename, typename> concept C0 = true;

template <typename T>
concept C =
requires(T t) {
  { 42 } -> C0<char [([] { return 42; }())]>;
};

static_assert(C<int>);

