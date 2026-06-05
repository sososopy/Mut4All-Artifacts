
namespace N {
  struct S {};
  void func(S);
}

namespace ADL {
  void func() = delete;
}

template <class T>
void call_func(T t) {
  using ADL::func;
  func(t);
}

int main() {
  call_func(N::S{});
}

