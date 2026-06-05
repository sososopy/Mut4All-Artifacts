
    template <typename T>
    static constexpr T xxx(){ return T(); }

    template <typename T>
    struct foo {
      using type = T(*)();
      static constexpr type value[1] = {&xxx<T>};
    };

    template <typename T>
    constexpr typename foo<T>::type foo<T>::value[1]; // fails
    //constexpr T (*foo<T>::value[1])(); // works

    int main() {
      constexpr int x = foo<int>::value[0](); // error here
    }

