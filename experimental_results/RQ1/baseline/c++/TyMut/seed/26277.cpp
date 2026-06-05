
struct Base {
  virtual ~Base();
};

struct Child: Base {
  void foo() {}
};

int main() {
  Child obj;
}
