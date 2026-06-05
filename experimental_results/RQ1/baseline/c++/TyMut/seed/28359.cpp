
template <class T>
class X {
  friend auto f(X);
};

struct Y : X<long> {
  friend auto f(X) { return 0L; }
};
