#pragma once

#include <list>
#include <string>

template <typename T>
struct Param {};

template <const auto&... Ts>
struct Configurator {};

// uncomment this for success
//#define USE_ABI_TAG

#ifdef USE_ABI_TAG
// succeeds with __attribute__((abi_tag("cxx11")))
__attribute__((abi_tag("cxx11"))) inline constexpr auto STRING_PARAM = Param<std::string>{};
__attribute__((abi_tag("cxx11"))) inline constexpr auto LIST_PARAM   = Param<std::list<int>>{};
#else
// fails without __attribute__((abi_tag("cxx11")))
inline constexpr auto STRING_PARAM = Param<std::string>{};
inline constexpr auto LIST_PARAM   = Param<std::list<int>>{};
#endif

using MyConfigurator = Configurator<STRING_PARAM, LIST_PARAM>;

void process(const MyConfigurator& configurator);

