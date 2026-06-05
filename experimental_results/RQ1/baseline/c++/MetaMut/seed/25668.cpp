
template <typename T>
struct X {
  X();
  mutable T x;
};

X<const int> a; // no error
X<int&> b;      // no error
