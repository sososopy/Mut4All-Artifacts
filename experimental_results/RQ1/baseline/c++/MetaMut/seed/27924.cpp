
template <class T> concept Int = __is_same_as (T, int);
static_assert (Int<char>);
