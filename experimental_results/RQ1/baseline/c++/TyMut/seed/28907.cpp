
//FILE: filler1.cc
module;
#include <memory>

export module filler1;


//FILE: filler2.cc

module;
#include <memory>

export module filler2;


//FILE: filler3.cc

module;
#include <memory>

export module filler3;


//FILE: vec.cc

module;
#include <memory>

export module vec;

export template<typename T> 
struct ivec{};


//FILE: test_impl.cc

module;
#include <memory>

import filler1;
import filler2;
import filler3;

import vec;

export module test_impl;

ivec<float> g_vec2;
