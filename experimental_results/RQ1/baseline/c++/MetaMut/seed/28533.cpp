
template <int...> struct index_tuple;
template <unsigned long, typename = index_tuple<>> struct build_number_seq;
template <unsigned long Num, int... Indexes>
struct build_number_seq<Num, index_tuple<Indexes...>>
    : build_number_seq<Num - 1, index_tuple<sizeof...(Indexes)>> {};
template <int... Indexes> struct build_number_seq<0, index_tuple<Indexes...>> {
  typedef index_tuple<Indexes...> type;
};
template <class> struct typelist;
template <int, typename> struct typelist_element;
template <typename Head, typename... Tail>
struct typelist_element<0, typelist<Head, Tail...>> {
  typedef Head type;
};
template <class> struct sizeof_typelist;
template <class... Types> struct sizeof_typelist<typelist<Types...>> {
  static const long value = sizeof...(Types);
};
template <class, class> struct invert_typelist_impl;
template <class Typelist, int... Ints>
struct invert_typelist_impl<Typelist, index_tuple<Ints...>> {
  static const long last_idx = sizeof_typelist<Typelist>::value - 1;
  typedef typelist<
      typename typelist_element<last_idx - Ints, Typelist>::type...>
      type;
};
template <class> struct invert_typelist;
template <class... Types> struct invert_typelist<typelist<Types...>> {
  typedef typename invert_typelist_impl<
      typelist<Types...>,
      typename build_number_seq<sizeof...(Types)>::type>::type type;
};
template <class> struct do_pack;
struct hook_defaults;
typedef do_pack<invert_typelist<typelist<hook_defaults>>::type> type;
