

namespace test {
  inline namespace internal {
    struct Type1 { };
  }
}

namespace test {
  struct Type1; //Type1 is actually in an inline namespace
}

template<typename> struct P { };

using Type = P<test::Type1>;

void f(Type& t);
