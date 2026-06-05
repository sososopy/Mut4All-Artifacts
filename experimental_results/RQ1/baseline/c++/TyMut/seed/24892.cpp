
// submod.mpp

module;

#include <iostream>

export module modA:submod;

// modA.mpp

module;

export module modA;

export import :submod;

// main.cpp

#include <string>

import modA;

std::string test_func() {
    return "";
}

