
template <long> using mp_size_t = int;
template <class...> struct mp_list;
template <class> struct mp_identity { using type = int; };
template <class... T> struct mp_inherit : T... {};
template <class T> using mpmf_wrap = mp_identity<T>;
template <class T> using mpmf_unwrap = typename T::type;
template <class, class> struct mp_map_find_impl;
template <template <class...> class M, class... T, class K> 
struct mp_map_find_impl<M<T...>, K> { 
  using U = mp_inherit<mpmf_wrap<T>...>;
  static mp_identity<void> f(mp_identity<int> *); 
  using type = mpmf_unwrap<decltype(f((U *)0))>;
};  
template <class M, class K> 
using mp_map_find = typename mp_map_find_impl<M, K>::type;
template <class> using mp_second = int;
template <class, int I> struct mp_at_c_impl { 
  using _map = mp_list<mp_list<>, int>;
  using type = mp_second<mp_map_find<_map, mp_size_t<I>>>;
};  
template <typename> using make_arg_list = mp_identity<void>;
template <typename Parameters, typename> struct argument_pack { 
  using type = typename mp_at_c_impl<
      typename make_arg_list<typename Parameters::parameter_spec>::type,
      0>::type;
}; 
struct parameters {
  typedef mp_list<> parameter_spec;
}; 
template <typename> 
using boost_param_result_39refine_mesh_3 = mp_identity<void>;
template <typename ParameterArgumentType0 = double> 
inline typename boost_param_result_39refine_mesh_3<
    typename argument_pack<parameters, ParameterArgumentType0>::type>::type
refine_mesh_3();
int main() { refine_mesh_3(); }
