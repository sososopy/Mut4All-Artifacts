
#include <cstdlib>

template<typename T>
void method(T* src, T* dst, size_t length);

template<>
void method<short>(short* src, short* dst, size_t length) {

}

template<>
void method<int>(int* src, int* dst, size_t length) {

}

