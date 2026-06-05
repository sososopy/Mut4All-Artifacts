
#include <cstdint>
#include <vector>
#include <iostream>

static const uint64_t MURMUR_PRIME = 0xc6a4a7935bd1e995ULL;
static const uint32_t MURMUR_SEED = 0xadc83b19ULL;

// Our hash function is MurmurHash2, 64 bit version.
// It was modified in order to provide the same result in
// big and little endian archs (endian neutral).
static uint64_t murmur_hash64A (const void* key, int32_t len, unsigned int seed) {
    const uint64_t m = MURMUR_PRIME;
    const int r = 47;
    uint64_t h = seed ^ (len * m);
    const uint8_t *data = (const uint8_t *)key;
    const uint8_t *end = data + (len-(len&7));

    while(data != end) {
        uint64_t k;
        k = *((uint64_t*)data);

        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
        data += 8;
    }

    switch(len & 7) {
    case 7: h ^= (uint64_t)data[6] << 48;
    case 6: h ^= (uint64_t)data[5] << 40;
    case 5: h ^= (uint64_t)data[4] << 32;
    case 4: h ^= (uint64_t)data[3] << 24;
    case 3: h ^= (uint64_t)data[2] << 16;
    case 2: h ^= (uint64_t)data[1] << 8;
    case 1: h ^= (uint64_t)data[0];
            h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}

void update_double(const std::vector<double>& values, std::vector<uint64_t>& hashes) {
    auto size = values.size();
    for (int i = 0; i < size; ++i) {
        auto v = values[i];
        uint64_t value = murmur_hash64A(&v, sizeof(v), MURMUR_SEED);
        hashes[i] = value;
    }
}

int main() {
    std::vector<double> values(3);
    std::vector<uint64_t> hashes(3);

    for (int i = 0; i < 3; ++i) {
        values[i] = i + 1;
    }
    update_double(values, hashes);
    for (auto hash : hashes) {
        std::cout << hash << std::endl;
    }
    return 0;
}

