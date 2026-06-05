
#include <iostream>

template <typename T> struct C
{ C(T) {
#ifdef DEBUG
  static_assert(false, "C<T> selected");
#endif
  std::cout << "C<T> selected"; }
};

template <> struct C<int>
{ C(int) { std::cout << "C<int> selected"; }
};

int main()
{ C c(1);
}

