
typedef int size_t;
template <typename> struct is_copy_constructible;
template <typename> struct remove_reference;
template <typename _Tp> struct remove_reference<_Tp &> { typedef _Tp type; };
template <typename> void forward();
template <size_t... _Indexes> struct A {
  typedef A<0, sizeof...(_Indexes)> __next;
};
template <size_t _Num> struct B {
  typedef typename B<_Num - 1>::__type::__next __type;
};
template <> struct B<0> { typedef A<> __type; };
template <typename _Tp, _Tp...> struct C;
template <typename, typename = B<1>::__type> struct D;
template <typename _Tp, size_t... _Idx> struct D<_Tp, A<_Idx...>> {
  typedef C<_Tp, _Idx...> __type;
};
template <typename _Tp, _Tp>
using make_integer_sequence = typename D<_Tp>::__type;
template <size_t... _Idx> using index_sequence = C<size_t, _Idx...>;
template <size_t> using make_index_sequence = make_integer_sequence<size_t, 0>;
template <int> void and_c();
template <class X> concept bool cpt_CopyConstructible() {
  return is_copy_constructible<X>::value;
}

template <class T> using uncvref_t = typename remove_reference<T>::type;
template <class...> class Tuple {};

auto make_tuple() { return Tuple<>(); }

template <class> size_t num_elements;
template <class... Values>
constexpr size_t num_elements<Tuple<Values...>> = sizeof...(Values);
template <size_t, class... TupleForwards> concept bool cpt_MappableElement() {
  return requires(TupleForwards... tuples){requires cpt_CopyConstructible<
      decltype(forward(forward<TupleForwards>(tuples)...))>()};
}

template <class...> int mappable_elements;
template <int... Indexes, class Functor, class... TupleForwards>
int mappable_elements<index_sequence<Indexes...>, Functor, TupleForwards...> =
    and_c<cpt_MappableElement<Indexes>()...>;
template <class...> int mappable_impl;
template <class Functor, class TupleForwardFirst, class... TupleForwardsRest>
int mappable_impl<Functor, TupleForwardFirst, TupleForwardsRest...> =
    mappable_elements<
        make_index_sequence<num_elements<uncvref_t<TupleForwardFirst>>>,
        TupleForwardFirst>;
template <class Functor, class... TupleForwards> concept bool cpt_Mappable() {
  return mappable_impl<Functor, TupleForwards...>;
}

cpt_Mappable { Functor, ... TupleForwards }

auto map(Functor, TupleForwards &&...);
auto t1 = make_tuple();
auto f1 = map(0, t1)