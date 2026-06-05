
#include <initializer_list>

struct _Iter_less_iter
{
  template<typename _Iterator1, typename _Iterator2>
  constexpr bool
  operator()(_Iterator1 __it1, _Iterator2 __it2) const
  { return *__it1 < *__it2; }
};

inline constexpr _Iter_less_iter
__iter_less_iter()
{ return _Iter_less_iter(); }

template<typename _ForwardIterator, typename _Compare>
constexpr _ForwardIterator
__min_element(_ForwardIterator __first, _ForwardIterator __last,
	      _Compare __comp)
{
  if (__first == __last)
    return __first;
  _ForwardIterator __result = __first;
  while (++__first != __last)
    if (__comp(__first, __result))
      __result = __first;
  return __result;
}

template<typename _ForwardIterator>
constexpr _ForwardIterator
inline min_element(_ForwardIterator __first, _ForwardIterator __last)
{
  return __min_element(__first, __last,
		       __iter_less_iter());
}

template<typename _Tp>
inline constexpr _Tp
min(std::initializer_list<_Tp> __l)
{ return *min_element(__l.begin(), __l.end()); }

int main()
{
  constexpr int foo = min({6, 4});
}

