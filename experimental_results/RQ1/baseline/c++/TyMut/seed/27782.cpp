
struct forward_iterator_tag { };
struct random_access_iterator_tag { };

template < typename _RandomAccessIterator,
           typename _Distance >
inline constexpr void
__advance (_RandomAccessIterator & __i, _Distance __n,
           random_access_iterator_tag)
{
  if (__builtin_constant_p (__n) && __n == 1)
    ++__i;
  else if (__builtin_constant_p (__n) && __n == -1)
    --__i;
  else
    __i += __n;
}
