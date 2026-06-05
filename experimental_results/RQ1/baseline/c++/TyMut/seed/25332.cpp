
class Heap;
class A {
public:  
  Heap *m_fn1();
};
template <typename> class B : A {
  void m_fn2() { m_fn1()->HashSeed; }
};

