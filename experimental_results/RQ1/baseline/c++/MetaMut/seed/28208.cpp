
template <int = 3> struct Vector {
  friend Vector Cross(const Vector &, const Vector &);
  Vector &rotate(const int &);
};
template <> Vector<> &Vector<>::rotate(const int &) {
  Vector __trans_tmp_8 = Cross(__trans_tmp_8, *this);
}
Vector<> Cross(const Vector<> &, const Vector<> &) {}
