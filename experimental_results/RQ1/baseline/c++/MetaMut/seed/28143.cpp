
struct M {
  explicit M(int);
  template <typename T> explicit M(T&&);
  M& a();
};

struct V {
  V(M m);
};

void addV(const V&);

int main() {
  addV(M(1).a());
}
