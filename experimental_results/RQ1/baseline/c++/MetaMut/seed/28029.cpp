
#include <memory>
#include <iostream>

#define NOINLINE __attribute__ ((noinline))

struct Deleter {    
    NOINLINE void operator()(void* p) {
        std::cout << "deleting: " << (uintptr_t)p << std::endl;
    }
};

NOINLINE std::unique_ptr<void, Deleter> get1() {
    return std::unique_ptr<void, Deleter>{(void*)1};
}
NOINLINE std::unique_ptr<void, Deleter> get2() {
    return std::unique_ptr<void, Deleter>{(void*)2};
}

NOINLINE void* test() {
    return (get1() ?: get2()).release();
}

int main() {
    uintptr_t result = (uintptr_t)test();
    std::cout << "result: " << result;
    return 0;
}
