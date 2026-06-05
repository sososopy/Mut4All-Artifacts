
#include <atomic>
static const std::memory_order relaxed = std::memory_order_relaxed;
extern std::atomic<int> eai;
void test_compare_exchange (int pi, int *pj)
{
#define cmpxchg(x, y, z, o1, o2) \
  std::atomic_compare_exchange_weak_explicit (x, y, z, o1, o2)
  cmpxchg (&eai, pi++, *pj++, relaxed, relaxed);


