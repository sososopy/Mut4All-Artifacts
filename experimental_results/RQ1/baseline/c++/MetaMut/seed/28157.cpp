
#include <cstdint>
#include <cstdio>
#include <new>
using std::size_t;
using std::uint32_t;
using std::uint64_t;

template<typename T, size_t N> struct myarray {
    T data[N];
};
myarray<uint32_t, 2> mul(uint32_t a, uint32_t b) noexcept {
    alignas(uint64_t) myarray<uint32_t, 2> ab;
#if 1 //no bug for #if 0
    ::new(&ab) uint64_t(uint64_t(a) * b);
#else
    uint64_t const c = uint64_t(a) * b;
    ab.data[0] = uint32_t(c); ab.data[1] = uint32_t(c >> 32);
#endif
    return ab;
}
struct bar : myarray<uint32_t, 2> {
    bar& operator+=(uint32_t a) noexcept {
        this->data[0] += a; //no bug when commented out
        this->data[1] += a; //no bug when commented out
        return *this;
    }
};
struct foo : myarray<uint32_t, 2> {
    foo() noexcept {
        static_cast<bar&>(*::new(this) myarray<uint32_t, 2>(mul(0xdeadbeef, 0xdeadbeef))) += 1;
    }
};
int main() {
    foo f;
    std::printf("0x%x, 0x%x\n", f.data[0], f.data[1]);
    std::printf("0x%x, 0x%x\n", 0x216da322, 0xc1b1cd13);
    return 0;
}
