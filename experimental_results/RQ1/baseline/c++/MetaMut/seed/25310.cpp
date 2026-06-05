void* operator new(unsigned long, void* p) { return p; }
void always_assert(bool);

struct IndirectReturn {
  IndirectReturn() {}
  // Make sure the indirect return value is never copied and RVO kicked in
  IndirectReturn(const IndirectReturn&) { always_assert(false); }
  IndirectReturn& operator=(const IndirectReturn&) = delete;
  ~IndirectReturn() {}
};

IndirectReturn foo();

void bar(void* ptr) {
  IndirectReturn x;

  new (ptr) IndirectReturn(foo());
}
