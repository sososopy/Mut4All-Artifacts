
inline int f() { return 42; }

template<class>
inline int v = f();

inline int g() { return v<int>; }



int main() {
  return v<int> != 42;
}

