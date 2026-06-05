
template <typename> struct __noexcept_move_assign_container;
template <class _Allocator> struct basic_string {
  basic_string &operator=(basic_string &&) noexcept(
      __noexcept_move_assign_container<_Allocator>::value);
  basic_string &assign(basic_string &&) noexcept(
      __noexcept_move_assign_container<_Allocator>::value);
};
template <class _Allocator>
basic_string<_Allocator> &
basic_string<_Allocator>::operator=(basic_string &&) noexcept(
    __noexcept_move_assign_container<_Allocator>::value) {}

