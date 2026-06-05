
struct alignas(8) Cell {};
struct TenuredCell : public Cell {};
struct BaseShape : public TenuredCell {
  void *p;
  unsigned q, r;
  void *s;
  __UINTPTR_TYPE__ t;
};
static_assert (sizeof (BaseShape) % 8 == 0, "");
