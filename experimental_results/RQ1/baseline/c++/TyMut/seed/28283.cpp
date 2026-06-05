

#include <atomic>
#include <cstdint>

uint64_t x;
void add_a() {
    x += 5;
}

std::atomic<uint64_t> y;

void add_b_non_atomic() {
    y.store(y.load(std::memory_order_relaxed) + 5, std::memory_order_relaxed);
}


