
namespace std {
template <typename _Tp, typename _Up> struct __replace_first_arg;
template <template <typename, typename...> class _Template, typename _Up,
          typename _Tp, typename... _Types>
struct __replace_first_arg<_Template<_Tp, _Types...>, _Up> {
  using type = _Template<_Up, _Types...>;
};
template <typename _Tp> struct allocator {
  typedef _Tp value_type;
  friend constexpr bool operator==(const allocator &,
                                   const allocator &) noexcept {
    return true;
  }
};
} // namespace std
namespace __gnu_cxx {
template <typename _Alloc, typename = typename _Alloc::value_type>
struct __alloc_traits {
  template <typename _Tp> struct rebind {
    typedef std::__replace_first_arg<_Alloc, _Tp> other;
  };
};
} // namespace __gnu_cxx
namespace std {
template <typename _Tp, typename _Alloc = std::allocator<_Tp>> class vector {
  typedef
      typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other
          _Tp_alloc_type;
};
} // namespace std

//foo.cpp

export module foo;
import <vector>;
export struct Foo {
  std::vector<int> v;
};
