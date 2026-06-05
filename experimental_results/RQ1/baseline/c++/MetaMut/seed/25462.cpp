
namespace {
template <typename> struct A {
  friend void foo() {}
};
struct C : A<int> {};
}

