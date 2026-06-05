
#include <typeinfo>
#if __cpp_lib_constexpr_typeinfo
constexpr bool b = typeid(int) == typeid(long);
#else
constexpr bool b = &typeid(int) == &typeid(long);
#endif

