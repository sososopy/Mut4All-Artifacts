
namespace std {

template <class _Iter> class __wrap_iter {
private:
  __wrap_iter(_Iter __x) {}

  template <class _CharT> friend class basic_string;
  template <class _Tp> friend class vector;
};

template <class _CharT> class basic_string {
public:
  typedef const char *const_pointer;
  typedef __wrap_iter<const_pointer> const_iterator;

  const_iterator begin() const {return const_iterator(const_pointer(0));}
};
typedef basic_string<char> string;

template <class _Tp> class vector {
public:
  typedef const _Tp *const_pointer;
  typedef __wrap_iter<const_pointer> const_iterator;

  const_iterator begin() const {return const_iterator(const_pointer(0));}
};

} // namespace std

void g(const std::vector<char> &v) { v.begin(); }

