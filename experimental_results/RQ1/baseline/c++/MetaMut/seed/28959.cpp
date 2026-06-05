
#include <iostream>
#include <string>
#include <type_traits>

class Base {
 public:
  template <class T>
  operator const T&() const {
    std::cout << "use template method" << std::endl;
    static T tmp{};
    return tmp;
  }

  virtual operator const std::string&() const {
    std::cout << "use overload method" << std::endl;
    const static std::string tmp;
    return tmp;
  }
};

template <class T>
class Derive : public Base {
 public:
  operator const T&() const override {
    using Y = std::string;
    static_assert(std::is_same<T, Y>::value, "");
    
    std::string static res;

    res = Base::operator const Y&();
    res = Base::operator const T&();
    return res;
  }
};

int main() {
  Derive<std::string> a;
  const std::string& b = a;
  (void)b;
}
