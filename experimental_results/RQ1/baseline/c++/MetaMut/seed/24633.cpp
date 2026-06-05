
template<class T>
struct L 
{
  T t;
  constexpr L(const T& t) : t(t) {}
  constexpr const T& value() const & { return t; }
  constexpr T& value() & { return t; }
};

template<class T>
constexpr T twice(const T& t) {
  L<T> l(t);
  l.value() *= 2;
  return l.value();
}

int main() {
  constexpr int i = twice(12);
  static_assert(i == 24, "");
}
