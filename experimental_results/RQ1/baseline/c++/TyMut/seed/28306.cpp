
template<class V>
class A {
  int y_;
  class B {
    class C {
      A* z_;
      friend bool operator==(C& x, int) {
        return x.z_->y_;
      }
    };
  };
};

