
template <typename n>
struct C {
  void find() {
    struct H {
        C<n> c{};
    };
    (void)[](auto){ return H{}; };
  }
};

void f() {
  C<int>().find();
}
