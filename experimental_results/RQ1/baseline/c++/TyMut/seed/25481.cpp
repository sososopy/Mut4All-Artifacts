
extern "C" {
  struct bah {
    template <typename T> struct bar {};
    template <typename T> void baz() {};
};
}
