
#include <typeinfo>

int main() {
    if (typeid(int()) == typeid(int() noexcept)) {
       throw "noexcept does not affect typeid";
    }
}

