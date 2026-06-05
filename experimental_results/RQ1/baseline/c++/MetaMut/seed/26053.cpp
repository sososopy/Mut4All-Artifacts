template<typename T>
class Class1;

namespace B {
  template<typename U>
  class Class2 {
    template<typename V>
    friend class Class1;
  private:
    int something_private;
  };
}

template<typename T>
class Class1
{
  void error() {
    B::Class2 b;
    b.something_private = 42;
  }
};
