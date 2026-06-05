

namespace std {
inline namespace __cxx11 {}
template <typename> class allocator;
namespace __cxx11 {
template <typename _CharT, typename = _CharT, typename = allocator<_CharT>>
class basic_string;
typedef basic_string<char> string;
}
template <typename> class allocator {
public:
  template <typename> struct rebind { typedef allocator other; };
};
template <typename _Alloc, typename _Tp> struct __alloctr_rebind {
  typedef typename _Alloc::template rebind<_Tp>::other __type;
};
template <typename _Alloc> struct allocator_traits {
  typedef decltype(0) pointer;
  template <typename _Tp>
  using rebind_alloc = typename __alloctr_rebind<_Alloc, _Tp>::__type;
};
}

namespace __gnu_cxx {
template <typename _Alloc>
struct __alloc_traits : std::allocator_traits<_Alloc> {
  typedef std::allocator_traits<_Alloc> _Base_type;
  template <typename _Tp> struct rebind {
    typedef typename _Base_type::template rebind_alloc<_Tp> other;
  };
};
}
namespace std {
namespace __cxx11 {
template <typename _CharT, typename, typename _Alloc> class basic_string {
  typedef
      typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_CharT>::other
          _Char_alloc_type;
  typedef __gnu_cxx::__alloc_traits<_Char_alloc_type> _Alloc_traits;

public:
  typedef _Char_alloc_type allocator_type;
  typedef typename _Alloc_traits::pointer pointer;
  struct _Alloc_hider {
    _Alloc_hider(pointer, _Alloc);
  } _M_dataplus;
  pointer _M_local_data();
  allocator_type _M_get_allocator();
  basic_string(const basic_string &)
      : _M_dataplus(_M_local_data(), _M_get_allocator()) {}
  basic_string(_CharT *, _Alloc __a = _Alloc())
      : _M_dataplus(_M_local_data(), __a) {}
};
}
}

auto vglambda = [](auto printer) {
  return [=](auto &&... ts) { return [=] { printer(ts...); }; };
};
main()

{
  vglambda([](auto, auto, auto) {})(std::string(""), "", "");
}
