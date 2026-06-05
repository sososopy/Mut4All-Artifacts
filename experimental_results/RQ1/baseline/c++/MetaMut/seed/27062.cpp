
#include <iostream>

template <typename T>
concept bool
JustEqualityComparable() {
  return requires(T a, T b) {
    { a == b } -> bool;
  };
}

template <typename T>
concept bool
JustInequalityComparable() {
  return requires(T a, T b) {
    { a != b } -> bool;
  };
}

bool operator==(const JustInequalityComparable& a1,
                const JustInequalityComparable& a2) {
  return !(a1 != a2);
}

bool operator!=(const JustEqualityComparable& a1,
                const JustEqualityComparable& a2) {
  return !(a1 == a2);
}

template <typename T>
  requires JustEqualityComparable<T>()
bool useEq(T t1, T t2) {
  return t1 == t2;
}

struct A {
  int a;
};

bool operator==(const A& a1, const A& a2) {
  return a1.a == a2.a;
}

int main() {
  using namespace std;

  cout << boolalpha << useEq(A{4}, A{6}) << "\n";

  return 0;
}

