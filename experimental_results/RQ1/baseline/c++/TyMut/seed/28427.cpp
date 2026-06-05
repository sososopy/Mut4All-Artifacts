
  namespace {
      template <class T>
      struct test {
          test(T) noexcept {}
      };

      UNUSED test(bool) -> test<bool>;
  }
