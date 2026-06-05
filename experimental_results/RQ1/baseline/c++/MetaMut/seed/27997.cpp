
  #include <cstring>
  struct S { S(); };
  void f() {
    S s[1];
    std::memset(s, 0, sizeof s);
  }

