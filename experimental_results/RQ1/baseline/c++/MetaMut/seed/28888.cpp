
#include <stdint.h>

typedef uint32_t __attribute__((__aligned__(1))) UnalignedUInt32;
typedef uint64_t __attribute__((__aligned__(1))) UnalignedUInt64;

uint32_t byteswap32(uint32_t x) noexcept {
  return (x << 24) | (x >> 24) | ((x << 8) & 0x00FF0000u) | ((x >> 8) & 0x0000FF00);
}

uint64_t byteswap64(uint64_t x) noexcept {
  return ((x << 56) & 0xff00000000000000) |
         ((x << 40) & 0x00ff000000000000) |
         ((x << 24) & 0x0000ff0000000000) |
         ((x <<  8) & 0x000000ff00000000) |
         ((x >>  8) & 0x00000000ff000000) |
         ((x >> 24) & 0x0000000000ff0000) |
         ((x >> 40) & 0x000000000000ff00) |
         ((x >> 56) & 0x00000000000000ff);
}

static inline void writeU64be(void* p, uint64_t val) {
  static_cast<UnalignedUInt64*>(p)[0] = byteswap64(val);
}

static inline uint32_t readU32be(const void* p) noexcept {
  uint32_t x = static_cast<const UnalignedUInt32*>(p)[0];
  return byteswap32(x);
}

// Returns 0xBB
uint32_t test_1() {
    uint8_t array[16] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    writeU64be(array + 6, 0xAABBCCDDEEFF1213);
    return array[7];
}

// Returns 0xCC
uint32_t test_2() {
    uint8_t array[16] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    writeU64be(array + 6, 0xAABBCCDDEEFF1213);
    return array[8];
}

// Returns 0xDD
uint32_t test_3() {
    uint8_t array[16] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    writeU64be(array + 6, 0xAABBCCDDEEFF1213);
    return array[9];
}

// Returns 0xEE
uint32_t test_4() {
    uint8_t array[16] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    writeU64be(array + 6, 0xAABBCCDDEEFF1213);
    return array[10];
}

// Returns 0708090A - the write has no effect when read with readU32be()
uint32_t test_u32() {
    uint8_t array[16] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    writeU64be(array + 6, 0xAABBCCDDEEFF1213);
    return readU32be(array + 7);
}

