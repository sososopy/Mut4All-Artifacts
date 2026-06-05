
struct A { };

template<class>
struct B {
  A a[1] = { A () };
};

int main () { B<void> b; }

