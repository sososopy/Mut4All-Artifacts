
template<typename R, typename Rep = double>
class quantity {};

template<template<typename, typename> typename Q>
inline constexpr bool valid_template_arguments = requires {
  requires requires { typename Q<int, int>; };
};
static_assert(valid_template_arguments<quantity>);
