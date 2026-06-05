
template<class T>
struct C {
  void f() { T x; }
  void g() = delete;
};
C<void> c;                      // OK, definition of C<void>​::​f is not instantiated at this point
template<> void C<int>::g() { } // error: redefinition of C<int>​::​g

int main(){}
