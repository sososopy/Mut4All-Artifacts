
struct T {
  template<typename T> void f(const T &v) {
    0 << v;
  }
};

namespace N {
  struct X {};
  struct Y : X {};
  void operator<<(int, const X&) {}
}

void operator<<(int, const N::Y&) = delete;

int main() {
  N::Y d;
  T().f(d);
}

