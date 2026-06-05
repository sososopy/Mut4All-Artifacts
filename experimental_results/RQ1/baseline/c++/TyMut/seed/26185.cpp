
#include <atomic>

struct Test1 {
    std::atomic<int> value2{0}; // no problem here
};

template <typename T> // T is not used
struct Test2 {
    std::atomic<int> value2{0}; // fails to compile
};

int main() {
    Test1 test;
    Test2<int> test2;
    return 0;
}
