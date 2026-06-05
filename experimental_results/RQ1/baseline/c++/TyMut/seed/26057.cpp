
#include <iostream>
using namespace std;

template<typename ... Pack>
void weeble (Pack ... Dims) {
    size_t r[] = {Dims...};
    for (int i = 0; i < sizeof...(Dims); ++i)
        std::cout << " " << r[i];
    std::cout << std::endl;
}

int main() {
    weeble(7,13,42,69);
}

