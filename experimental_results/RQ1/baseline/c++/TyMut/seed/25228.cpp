
class A {
public:
  template <typename> void As();
  static A *FromWebContents();
};
template <typename T> class B : A {
  void FromWebContents() {
    auto guest = A::FromWebContents();
    guest ? guest->As<T>() : nullptr;
  }
};

