
#include <iostream>

int main(){
  auto call_cart{[=](auto f, auto x, auto...rest) {
                (((x < rest) ? f(x, rest) : (void) 0), ...);
            }};
  auto cartesian{[=](auto...xs) {
    return [=](auto f) {
      (call_cart(f, xs, xs... ), ...); // ok with gcc8 // error with gcc9 
      // other version  without fold expression // same problem
      // (void)std::initializer_list<int>{((void)call_cart(f,xs,xs...),0)...};
    };
  }};
  
  constexpr auto print_cart{cartesian(1, 2, 3)};

  static_assert((print_cart([](int x, int y)constexpr{}),true));
  print_cart([](int x, int y) { std::cout << x << "," << y << std::endl; });
}
