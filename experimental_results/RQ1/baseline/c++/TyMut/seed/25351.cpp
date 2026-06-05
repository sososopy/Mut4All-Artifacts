
#include <thread>
class X {
    int i;
};
main() {
    X x;
    std::thread([&x] { x.i = 3; });
}

