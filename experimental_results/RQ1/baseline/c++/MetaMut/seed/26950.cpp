#include <iostream>

struct a {
    static const int i=0;
};

namespace x {
    namespace a {
        int i;
    }
}

using namespace x;

int main()
{
    a::i=1;
    std::cout << "i = " << a::i;
    return 0;
}

