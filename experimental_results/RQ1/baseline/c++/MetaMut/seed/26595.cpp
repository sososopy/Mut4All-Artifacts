
#include <type_traits>
struct a;
struct b{};
bool c = std::is_convertible<a,b>::value;
