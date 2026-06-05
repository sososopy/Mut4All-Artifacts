
module;

#include <string>

export module modA;


#include <string>

import modA;

std::string test_func() {
    return "foo";
}

int main() {
    return 0;
}
