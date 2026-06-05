
void __throw_bad_array_new_length();
template <typename> void forward();
template <typename> struct __new_allocator {
  int allocate(long __n, const void * = 0) {
    if (__builtin_expect(__n, false))
      if (__n)
        __throw_bad_array_new_length();
  }
};
template <typename> struct allocator_traits;
template <typename _Tp> struct allocator_traits<__new_allocator<_Tp>> {
  using allocator_type = __new_allocator<_Tp>;
  using pointer = _Tp;
  using size_type = long;
  static pointer allocate(allocator_type &__a, size_type __n) {
    __a.allocate(__n);
  }
};
struct __alloc_traits : allocator_traits<__new_allocator<int>> {};
template <typename, typename, typename, typename, typename = int>
struct _Rb_tree {
  struct _Alloc_node {
    _Alloc_node(_Rb_tree);
    template <typename _Arg> void operator()(_Arg) {
      __new_allocator<int> __trans_tmp_1;
      __alloc_traits::allocate(__trans_tmp_1, 1);
    }
  };
  template <typename _Arg, typename _NodeGen>
  int _M_insert_(int, int, _Arg &&, _NodeGen &);
  template <typename _Arg> int _M_insert_unique(_Arg &&);
};
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
template <typename _Arg, typename _NodeGen>
int _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert_(
    int, int, _Arg &&, _NodeGen &__node_gen) {
  __node_gen(forward<_Arg>);
}
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
template <typename _Arg>
int _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert_unique(
    _Arg &&) {
  int __res_1, __res_0;
  _Alloc_node __an(*this);
  _M_insert_(__res_0, __res_1, forward<_Arg>, __an);
}
struct set {
  _Rb_tree<int, int, int, int> _M_t;
  int insert___x;
  void insert() { _M_t._M_insert_unique(insert___x); }
};
main() {
  set the_set;
  the_set.insert();
}
