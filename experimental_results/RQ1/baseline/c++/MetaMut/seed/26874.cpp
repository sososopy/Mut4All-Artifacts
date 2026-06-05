

template <class T>
  requires __is_same_as(T, double)
union A {};

int main() { A<int>{}; }

despite that int does not satisfy the constraints of A. Changing the expression __is_same_as(T, double) to true, false, 42, or (new T) has no effect - the program compiles without diagnostics.

Compiling *this* program, however:

template <class>
union B {};

template <class T>
  requires false
union B<T> {};

int main() {
  B<int>{};
}
