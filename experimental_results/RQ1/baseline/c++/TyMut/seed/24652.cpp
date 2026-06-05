
struct C {
  C(const C &);
};

struct X {
  operator C() const;
};

C a{X()};

