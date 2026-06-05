
namespace std {
template <typename _Tp> struct A { static constexpr _Tp value = 1; };
template <typename, typename> struct is_same;
template <typename, typename _To> struct __is_convertible_helper {
  template <typename> static A<bool> __test();
  typedef decltype(__test<_To>()) type;
};
template <typename, typename>
struct is_convertible : __is_convertible_helper<int, int>::type {};
}
namespace detail_variadic_operator {
template <int...> struct and_c_impl { static constexpr int value = 1; };
}
template <int> constexpr bool and_c() {
  return detail_variadic_operator::and_c_impl<>::value;
}
namespace htl {
namespace empty_base_class_namespace {
template <class, class> struct Pack {
  template <class OtherValue> Pack(OtherValue);
};
template <class Tag, class Value> Value &unpack(Pack<Tag, Value>);
}
using empty_base_class_namespace::Pack;
namespace detail_tuple {
struct TupleBase {
  TupleBase(int);
};
}
template <class...> class Tuple : detail_tuple::TupleBase {
  using TupleBase::TupleBase;
};
}
using index_t = int;
template <class X> concept bool cpt_DynamicIndex() {
  return std::is_same<X, index_t>::value;
}
template <class X> concept bool cpt_Index() { return cpt_DynamicIndex<X>(); }
template <class X> concept bool cpt_Extent() {
  return std::is_convertible<X, index_t>::value;
}
template <class... Extents>
requires and_c<cpt_Index<Extents>()...>() class Shape;
namespace detail_concept {
template <class> int match_contiguous_shape;
template <class... Extents>
constexpr bool match_contiguous_shape<Shape<Extents>> = 1;
}
template <class X> concept bool cpt_ContiguousShape() {
  return detail_concept::match_contiguous_shape<X>;
}
template <class> using extent_type = decltype(0);
namespace detail_dimensionality {
struct extents_tag;
}
template <class... Extents>
class Dimensionality
    : htl::Pack<detail_dimensionality::extents_tag, htl::Tuple<Extents...>> {
  using Base =
      htl::Pack<detail_dimensionality::extents_tag, htl::Tuple<Extents...>>;

public:
  Dimensionality(Extents... extents) : Base(htl::Tuple<>(extents...)) {}
  decltype(auto) extents() { return unpack(*this); }
};
template <class... Extents>
requires and_c<cpt_Index<Extents>()...>() class Shape
    : public Dimensionality<Extents...> {
  using Base = Dimensionality<Extents...>;

public:
  Shape(Extents... extents) : Base(extents...) {}
};
template <cpt_Extent... Extents> auto make_shape(Extents... extents) {
  return Shape<extent_type<Extents>...>(extents...);
}
template <cpt_ContiguousShape Shape> auto make_subshape(Shape shape) {
  return shape.extents();
}
auto shape1 = make_shape(0);
auto subshape2 = make_subshape(shape1);
