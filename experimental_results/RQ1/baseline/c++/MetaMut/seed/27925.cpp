
namespace my {

int foo() { return 1; }

struct s {

  void bar(int i) {
    auto *objects = reinterpret_cast<char(*)[foo()]>(storage);
    void *ptr = &objects[i];
  }

  void baz();

  char *storage;
};

void s::baz() {
  bar(0);
}

} // namespace my

