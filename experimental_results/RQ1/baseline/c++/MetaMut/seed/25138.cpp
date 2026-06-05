
#include <iostream>

static const auto add = [](auto a, auto b) {return a + b;};
static const auto sub = [](auto a, auto b) {return a - b;};
static const auto mul = [](auto a, auto b) {return a * b;};
static const auto frac = [](auto a, auto b) {return a / b;};

template <class A, class B, class F> class BinaryOP;

template <class A, class B> using Addition = BinaryOP <A, B, decltype(add)>;
template <class A, class B> using Subtraction = BinaryOP <A, B, decltype(sub)>;
template <class A, class B> using Multiplication = BinaryOP <A, B, decltype(mul)>;
template <class A, class B> using Division = BinaryOP <A, B, decltype(frac)>;

static const auto make_binop = [](auto a, auto b, auto F) { return BinaryOP<decltype(a), decltype(b), decltype(F)>(a, b, F); };
static const auto make_add = [](auto a, auto b) { return make_binop(a, b, add); };
static const auto make_sub = [](auto a, auto b) { return Subtraction<decltype(a), decltype(b)>(a, b, sub); };
static const auto make_mul = [](auto a, auto b) { return Multiplication<decltype(a), decltype(b)>(a, b, mul); };
static const auto make_div = [](auto a, auto b) { return Division<decltype(a), decltype(b)>(a, b, frac); };

template <class A, class B, class F>
class BinaryOP {
protected:
  typedef BinaryOP <A, B, F> THIS_T;
  A lhs;
  B rhs;
  F func;
public:
  BinaryOP(A lhs, B rhs, F &func):
    lhs(lhs), rhs(rhs), func(func)
  {}
  virtual ~BinaryOP()
  {}
  template <class T>
  operator T() {
    return func(T(lhs), T(rhs));
  }
  template <class T>
  Addition<THIS_T, T> operator+(const T &other) {
    return make_add(*this, other);
  }
  template <class T>
  Subtraction<THIS_T, T> operator-(const T &other) {
    return make_sub(*this, other);
  }
  template <class T>
  Multiplication<THIS_T, T> operator*(const T &other) {
    return make_mul(*this, other);
  }
  template <class T>
  Division<THIS_T, T> operator/(const T &other) {
    return make_div(*this, other);
  }

  friend std::ostream &operator<<(std::ostream &os, const THIS_T &bin) {
    os << "(binop: " << bin.lhs << ", " << bin.rhs << ")";
    return os;
  }
};

int main() {
  std::cout << make_add(2, 3) + make_sub(5, 6) * make_add(3, 0) << std::endl;
}
