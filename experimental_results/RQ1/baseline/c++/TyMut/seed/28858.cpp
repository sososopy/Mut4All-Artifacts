
struct Thing0 {
  volatile int a;

  constexpr Thing0( int v ): a{v} {}
  constexpr Thing0(Thing0 const& other) : a(other.a) {}
  constexpr Thing0& operator=(Thing0 const& rhs) {
    a = rhs.a;
    return *this;
  }
  ~Thing0() = default;
};

Thing0 func0() { return Thing0{5}; }

