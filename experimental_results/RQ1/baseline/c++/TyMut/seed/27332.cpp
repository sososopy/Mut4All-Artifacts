
struct integral_constant {};
template <bool> using __bool_constant = integral_constant;
template <typename _Tp, typename _Up>
using __is_nothrow_assignable_impl =
    __bool_constant<__is_nothrow_assignable(_Tp, _Up)>;
template <typename _Tp>
struct __is_nt_move_assignable_impl : __is_nothrow_assignable_impl<_Tp, _Tp> {};
template <int _Len> struct aligned_storage {
  struct type {
    char __data[_Len];
  };
};
template <long _Len>
using aligned_storage_t = typename aligned_storage<_Len>::type;
template <int bufferSize> class PolymorphicSmallObject {
  aligned_storage_t<bufferSize> buffer_;
};
template <class Obj> void test() { __is_nt_move_assignable_impl<Obj> value; }
int main() { test<PolymorphicSmallObject<0>>; return 0; }

