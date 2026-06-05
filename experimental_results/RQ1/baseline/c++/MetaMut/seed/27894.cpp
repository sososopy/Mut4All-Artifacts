
#pragma once

namespace shared {

template<class T>
struct A {
    static thread_local unsigned a0;

    static unsigned* f1() {
        thread_local unsigned a1 = 0;
        return &a1;
    }

    static unsigned* f2() {
        static unsigned a2 = 0;
        return &a2;
    }
};

template<class T>
thread_local unsigned A<T>::a0 = 0;

unsigned* f0();
unsigned* f1();
unsigned* f2();

} // namespace shared

$ cat shared.cc
#include "shared.h"

unsigned* shared::f0() {
    return &A<void>::a0;
}

unsigned* shared::f1() {
    return A<void>::f1();
}

unsigned* shared::f2() {
    return A<void>::f2();
}

$ cat test.cc
#include "shared.h"
#include <cstdio>
#include <dlfcn.h>

int main() {
    using namespace shared;

    std::printf("main a0: %p\n", &A<void>::a0);
    std::printf("main a1: %p\n", A<void>::f1());
    std::printf("main a2: %p\n", A<void>::f2());

    void* so = ::dlopen("./libshared.so", RTLD_NOW | RTLD_GLOBAL);
    auto pf0 = reinterpret_cast<decltype(&f0)>(::dlsym(so, "_ZN6shared2f0Ev"));
    auto pf1 = reinterpret_cast<decltype(&f1)>(::dlsym(so, "_ZN6shared2f1Ev"));
    auto pf2 = reinterpret_cast<decltype(&f2)>(::dlsym(so, "_ZN6shared2f2Ev"));

    std::printf("shared a0: %p\n", pf0());
    std::printf("shared a1: %p\n", pf1());
    std::printf("shared a2: %p\n", pf2());
}

