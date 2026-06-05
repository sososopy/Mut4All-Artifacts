
    template<int> struct S {};

    template<int N>
    constexpr auto V = S<N>{};
    // constexpr S<N> V{}; // This works

    template<typename T>
    auto f(T) {
      return V<(
    #if defined(TRIGGER_ERROR)
          (void)T(),
    #endif
      0)>;
    }

    int main() {
      f(0);
    }

