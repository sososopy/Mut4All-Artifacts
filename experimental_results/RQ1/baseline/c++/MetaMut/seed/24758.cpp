
module;

namespace std {
template<class _Tp>
void swap(_Tp& a, _Tp& b);

namespace __exception_ptr {
class exception_ptr
{};
void swap(exception_ptr&, exception_ptr&);
}
using __exception_ptr::swap;

template < typename _Tp> struct shared_ptr {
  void swap(shared_ptr& other) { std::swap(_M_ptr, other._M_ptr); }
  _Tp *_M_ptr;
};

struct future {
  shared_ptr< void > _M_state;
  void operator=(future& other) { _M_state.swap(other._M_state); }
};
} // std

export module std;
namespace std {
using std::swap;
}

