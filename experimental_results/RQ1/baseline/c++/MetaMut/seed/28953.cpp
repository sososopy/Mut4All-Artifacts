
// a-m.cc
module;

#include <string>

export module a;

export std::string do_a() {
        return "abc";
}

// main.cc
#include <iostream>

import a;

int main() {
        std::cout << do_a() << "\n";
        return 0;
}

