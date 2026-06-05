
template<int targ_id, typename t_parameter,
         template<typename T, T&> typename tt_w_data>
struct tt_main {
  static t_parameter m_parameter;
  template<template<typename T, T&> typename t_data> using t_make
     = t_data<int, m_parameter>;
  using t_data = t_make<tt_w_data>;
};
template<typename T, T&> struct P {};
tt_main<0, int, P> a;

