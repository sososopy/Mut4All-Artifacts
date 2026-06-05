
template<class>
concept Auto = true;

template<class T, class U>
struct X { constexpr X(const Auto auto) {} };

template<class U>
using Y = X<long, U>;

constexpr Y y{1};
