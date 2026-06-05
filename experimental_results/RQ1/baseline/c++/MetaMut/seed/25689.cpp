
namespace test {
  namespace foo {
    void f();
  }
  namespace foo {
    void f() {};
  }
#ifdef BAR
}
namespace test {
#endif
  namespace foo __attribute__((visibility("hidden"))) {
  }
}
