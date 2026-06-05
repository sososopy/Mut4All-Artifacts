#include <cstddef>
int operator""_s(const char32_t *a, size_t b){
    return 0;
}
int f(){
    using a=decltype(U"\x1181"_s);
    using b=decltype(U"\x8111"_s);
    using c=decltype(U" \x1181"_s);
}
