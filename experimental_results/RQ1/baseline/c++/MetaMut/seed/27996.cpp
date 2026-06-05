
struct S;
template<int> struct T
{
  static_assert (__builtin_has_attribute (((S *) 0) -> a, packed));
};

