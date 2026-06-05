
class A;
template <typename> struct B {
  struct C {
    A *_M_end_of_storage;
  };
  ~B() { _M_impl._M_end_of_storage - 0; }
  C _M_impl;
};

class D {
  virtual ~D();
};
class F {
  B<int> m_value;
};
class G : D {
  F NPCadesImpl;
};


