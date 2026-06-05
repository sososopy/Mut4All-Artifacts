
#include <cstdlib>
#include <cassert>
#include <stdio.h>

void* operator new[](std::size_t sz){
    printf("new called\n");
    void* m = malloc(sz);
    assert(m);
    return m;
}

void operator delete[](void*p)noexcept {
    printf("usual deallocation called\n");
    return free(p);
}

void operator delete[](void*p, std::size_t size)noexcept {
    printf("sized deallocation called\n");
    return free(p);
}

void test() {
    delete[] new int[3];
}

int main() {
    test();
}
