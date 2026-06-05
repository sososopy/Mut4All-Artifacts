
namespace std {
  struct strong_ordering {
  };
}

struct F {
  [[deprecated("oh no")]] std::strong_ordering operator<=>(const F&) const = default;
};
void use_f(F f) {
  void(f == f);
}
