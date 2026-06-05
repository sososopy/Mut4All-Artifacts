
  #include <type_traits>
  #include <cstring>

  class Good {
   public:
      Good & operator=(Good const &) = default;
  };

  class Bad {
   public:
      Bad & operator=(Bad const &) & = default;
  };

  template<typename T>
  void test() {
      static_assert(std::is_trivially_copyable_v<T>);
      T value1;
      T value2;
      std::memcpy(&value1, &value2, sizeof(T));
  }

  int main() {
      test<Good>();
      test<Bad>();
  }
