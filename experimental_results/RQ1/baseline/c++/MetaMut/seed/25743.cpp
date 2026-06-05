
#include <type_traits>

template <typename F, typename S>
struct TypePair {
  typedef F First;
  typedef S Second;
};

template <int I>
class Base {
 public:
  typedef TypePair<Base, Base> Family;
  typedef Base ParentType;
};

template <int I>
class Derived : public Base<I> {
 public:
  typedef TypePair<Derived, typename Derived::Family::First> Family;
  typedef typename Family::Second ParentType;
};

static_assert(std::is_same<Base<1>, Derived<1>::ParentType>::value, "Error");



