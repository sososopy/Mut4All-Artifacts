
#include <iostream>
#include <cmath>
#include <cstddef>
#include <cstdint>

static const uint32_t ONE_BILLION = 1000000000;

static const uint32_t MURMUR3_32_SEED = 104729;

static uint32_t rotl32(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

static uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

// modify from https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
static uint32_t murmur_hash3_32(const void* key, int32_t len, uint32_t seed) {
    const uint8_t* data = (const uint8_t*)key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const uint32_t* blocks = (const uint32_t*)(data + nblocks * 4);

    for (int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t* tail = (const uint8_t*)(data + nblocks * 4);
    uint32_t k1 = 0;
    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    };

    h1 ^= len;
    h1 = fmix32(h1);
    return h1;
}

class DecimalV2Value {
public:
    DecimalV2Value() = default;
    inline bool from_olap_decimal(int64_t int_value, int64_t frac_value) {
        bool success = true;
        bool is_negative = (int_value < 0 || frac_value < 0);
        if (is_negative) {
            int_value = std::abs(int_value);
            frac_value = std::abs(frac_value);
        }

        if (frac_value > 9) {
            frac_value = 9;
            success = false;
        }

        _value = static_cast<__int128_t>(int_value) * ONE_BILLION + frac_value;
        if (is_negative) _value = -_value;

        return success;
    }

private:
    __int128_t _value;
};

struct DecimalFindOp {
    uint32_t find_olap_engine(const void* data) {
        DecimalV2Value value;
        int64_t int_value = *(int64_t*)(data);
        int32_t frac_value = *(int32_t*)((char*)data + sizeof(int64_t));
        value.from_olap_decimal(int_value, frac_value);
        return murmur_hash3_32((char*)&value, sizeof(DecimalV2Value), 0);
    }
};

struct decimal12_t {
    int64_t integer;
    int32_t fraction;
} __attribute__((packed));

int main(int argc, char* argv[]) {
    decimal12_t d12;
    d12.integer = atoi(argv[0]);
    d12.fraction= atoi(argv[1]);
    DecimalFindOp op;
    std::cout << op.find_olap_engine(&d12) << std::endl;
    return 0;
}
