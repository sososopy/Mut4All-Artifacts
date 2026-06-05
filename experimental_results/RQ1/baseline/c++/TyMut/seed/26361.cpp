
template<template<auto, auto> typename> void takes_templ();

template<typename>
struct v_v_member_templ_fn {
    template<auto, auto>
    using fn = void;
};

using ice = decltype(takes_templ<v_v_member_templ_fn<void>::template fn>());

