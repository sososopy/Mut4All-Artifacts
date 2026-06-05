#include <cstddef>

class Test {
 public:
  // The problem is the `const` qualifier. If it is removed, then the frontend does not crash.
  void* operator new(size_t bytes) const { return nullptr; }
};

int main() {
  new Test();
  return 0;
}