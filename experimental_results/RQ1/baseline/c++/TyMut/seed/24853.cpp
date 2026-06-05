
template <class _Tp> struct __has_storage_type {};
template <class _Cp, bool = __has_storage_type<_Cp>::value>
class __bit_reference {};
template <class _Cp> class __bit_reference<_Cp, false> {
  friend class __bit_reference<_Cp>;
};

