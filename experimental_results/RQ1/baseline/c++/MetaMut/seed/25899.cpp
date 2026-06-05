
struct AtomicInt
{
  _Atomic(int) Atomic;
};
AtomicInt GlobalAtomic0 = {0};
constexpr AtomicInt GlobalAtomic1 = {0};

