
inline void* operator new(decltype(sizeof(0)), void* ptr) {
  return ptr;
}

struct X { int x; int y; int z = 42; };

void test_bar(X* p) {
  new(p) X{};   // Bad.
}

