
#include <vector>

template <class T>
struct S {
  ~S() {
    for (size_t i = 0; i < sources_.size(); ++i)
      sources_[i]->frob();
  }
 private:
  std::vector<T*> sources_;
};

class C;

struct T {
  S<C> s_{};
};
