
namespace test6 {
  template<typename T> constexpr T f(T);
  template<typename T> constexpr T g(T t) {
    typedef int arr[f(T())];
    return t;
  }
  template<typename T> constexpr T f(T t) {
    typedef int arr[g(T())];
    return t;
  }
  int n = f(0);
}

