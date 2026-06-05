
module;

#include <iostream>

export module modA:submod;



module;

export module modA;

export import :submod;



import modA;

std::string test_func() {
    return "";
}


