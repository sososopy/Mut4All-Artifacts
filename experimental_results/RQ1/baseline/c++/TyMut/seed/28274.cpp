template <class M>
struct L: M {
  using M::a;
  void p() { this->template a<>; }
};

