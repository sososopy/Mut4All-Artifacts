

template <typename Element> struct box {
  Element value;
  constexpr box() : value{} {}
};
struct B: box<int> { };
template box<B>::box();
