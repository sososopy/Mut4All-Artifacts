
export module mymod;

template <typename, typename>
struct _Hashtable;

export
template <typename _Key, typename _Val>
struct _Map_base {
  void f() {
    _Hashtable<_Key, _Val> __h;
    __h._M_hash_code(0);
  }
};

template <typename _Key, typename _Value>
struct _Hashtable {
  template <typename, typename> friend struct _Map_base;
protected:
  void _M_hash_code(int);
};

_Hashtable<int, int> m;

//$ cat testcase_b.C
import mymod;

int main() {
  _Map_base<int, int> m;
  m.f();
}

