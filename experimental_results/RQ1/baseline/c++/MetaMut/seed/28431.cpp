

struct A {
  A(int aa) : a(aa) {}

  A(const A &a) = delete;
  A &operator=(const A &a) = delete;

private:
  int a;
  std::vector<int> v;
};


struct B2 {
  B2()
    : a2{{1}, {2}} // -> error: use of deleted function 'A::A(const A&)' (gcc 9.3)
  {} 

private:
  A a2[2];
};

