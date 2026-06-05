

struct C1 { void operator+(); };
struct C2 { void operator+(); };
struct C3 : C1, C2 {
  template <class T> void get() { [] (T x) { +x; }; }
};
