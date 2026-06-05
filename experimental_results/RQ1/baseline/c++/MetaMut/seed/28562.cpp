
class Matrix {
public:
  Matrix();
  template <typename OtherDerived> Matrix(OtherDerived);
};
template <class = double> using Vector = Matrix;
class L1TightFrame {
protected:
  template <class T0, class T1> void operator()(T0, double, T1);
};
class L1 : L1TightFrame {
public:
  template <class... T>
  auto tight_frame(T...) -> decltype(operator()(T()...)) {}
};
template <class> class ImagingProximalADMM {
public:
  template <class DERIVED> ImagingProximalADMM(DERIVED);
  L1 l1_proximal() const;
  int operator()() {
    int result, __trans_tmp_10;
    Vector<> out, __trans_tmp_12;
    operator()(out, __trans_tmp_10, __trans_tmp_12);
    return result;
  }
  int operator()(Vector<> &, Vector<> const &, Vector<> const &) const;
  template <class T0, class T1>
  void l1_proximal(T0 out, double gamma, T1 x) const {
    l1_proximal().tight_frame(out, gamma, x);
  }
};
template <class SCALAR>
int ImagingProximalADMM<SCALAR>::operator()(Vector<> &, Vector<> const &,
                                            Vector<> const &) const {
  int result;
  [&](Vector<> out, double gamma, Vector<> x) { l1_proximal(out, gamma, x); };
  return result;
}
Vector<> y = ImagingProximalADMM<double>(y)();

