
template<template<const int&> typename tt_w_data>
struct tt_main {
  static int m_parameter;
  template<template<const int&> typename t_data> using t_make 
    = t_data<m_parameter>;
};

