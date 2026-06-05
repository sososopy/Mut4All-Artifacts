

#include <unordered_map>

struct X {
    static X * fromString();

    X(int x, int y);

    static const std::unordered_map<int, X> var;
};


std::unordered_map<int, X> X::var = {
    {0, X(0, 0)},
};


X * X::fromString() {
    std::unordered_map<int, X>::iterator it = var.find(0);
    if (it == var.end()) return 0;
    return &(it->second);
}

