
struct A { };
A& operator<<(A&, const char*);

struct B {
  int size();
};

struct C { };

template <class S, class T>
S bar(const S& s, const T& t) {
  return s;
}

template<class S, class T>
void foo() {
  A a;
  B b;
  a << bar(b.size(), C());
}
