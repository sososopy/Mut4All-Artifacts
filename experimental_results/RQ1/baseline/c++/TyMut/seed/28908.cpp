template <typename T> struct X { X(T t) {} };

int main() {
  auto t00 = X(1);
  auto t01 = X{1};
  X t02{1};
  X t03(1);

  auto t04 = X(X{1});
  auto t05 = X{X{1}};
  auto t06 = X(X(1));
  auto t07 = X{X(1)};
  X t08(X{1}); // GCC 11.x and up rejects this; MSVC and Clang accept it.
  X t09{X{1}};
  X t10(X(1));
  X t11{X(1)};
}

