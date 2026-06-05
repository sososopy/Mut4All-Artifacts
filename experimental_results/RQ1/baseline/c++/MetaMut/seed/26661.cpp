

#include <map>

template <typename T>
struct X {
    std::map<int, T>* storage = new std::map<int, T>;
};

int main() {
    X<int> x;
}

