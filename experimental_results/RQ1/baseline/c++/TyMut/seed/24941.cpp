
struct Optional {
  constexpr Optional() : _dummy{} { _value = 1; }
  union {
    int _dummy;
    int _value;
  };
};
Optional opt{};

