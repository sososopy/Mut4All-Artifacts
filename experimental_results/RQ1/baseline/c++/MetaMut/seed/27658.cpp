
namespace std {
  struct strong_ordering {
  };
}

namespace Synth {
  struct A {
    friend bool operator==(A, A) noexcept;
    friend bool operator<(A, A) noexcept;
  };
  struct B {
    A a;
    friend std::strong_ordering operator<=>(B, B) = default;
  };
  std::strong_ordering operator<=>(B, B) noexcept;
}

