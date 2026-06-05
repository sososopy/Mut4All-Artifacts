template <typename T>
struct _Copy_ctor_base : T {};

template <typename _Types>
struct _Variant_base : _Copy_ctor_base<_Types> {};

struct monostate {};

template <>
struct monostate {};

_Variant_base<monostate> v;