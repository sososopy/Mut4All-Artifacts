
template <typename> bool is_nothrow_constructible_v;
template <typename> struct QArrayExceptionSafetyPrimitives {
  struct {
    template <typename> using iterator_move_value = decltype(0);
    template <typename ForwardIt>
    void
    move() noexcept(is_nothrow_constructible_v<iterator_move_value<ForwardIt>>);
  };
};
