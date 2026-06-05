
class A {
  public:
    template<typename T>
      friend int f(int x, T v) {
        return x + v;
      }
};


template<>
int f(int x, int v) {
  return x + v;
}

