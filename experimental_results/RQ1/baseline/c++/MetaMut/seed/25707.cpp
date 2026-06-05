# 1 "test.cpp" 1
class C1 {};

template <class C>
  class C2
  {
    static C1 p __attribute__((visibility("hidden")));
  };
  C1 f() {return C2<int>::p;}
