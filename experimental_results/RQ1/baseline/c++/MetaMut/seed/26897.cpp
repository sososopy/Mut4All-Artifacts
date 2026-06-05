

namespace std
{
    template<typename _FIter, typename _Tp>
      _FIter
      remove(_FIter, _FIter, const _Tp&);
}

extern "C" {
extern int remove (const char *__filename) throw ();
}

namespace std
{ 
  using ::remove;
  
  template<typename _ForwardIterator, typename _Tp>
    inline _ForwardIterator 
    remove(_ForwardIterator __first, _ForwardIterator __last,
    const _Tp& __value)
    { 
      return __first;
    }
}

namespace std
{
  struct value_type { };
  typedef value_type* iterator_type;

  template iterator_type remove(iterator_type, iterator_type,
    const value_type&);
}
