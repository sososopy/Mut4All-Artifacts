

struct has_ctor_and_dtor {
  has_ctor_and_dtor() {}
  ~has_ctor_and_dtor() {}  
};

typedef has_ctor_and_dtor (*fptr)();

int main() {
  const fptr func = []() {
    return has_ctor_and_dtor();
  };
  func();
  return 0;
}
