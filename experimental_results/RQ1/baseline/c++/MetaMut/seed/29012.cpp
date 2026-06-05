
template< class T >
struct probe { static constexpr bool value = false; };
//
template< class R, class C >
struct probe<R C::*> { static constexpr bool value = true; };

export
template< class T >
concept
  mbr_ptr = probe<T>::value;
