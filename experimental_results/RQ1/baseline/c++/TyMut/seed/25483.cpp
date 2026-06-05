# 1 "one-body-sample-83GwrP.ii" 1
template<int N1> struct Dummy000;
template<typename C1> struct Dummy111;
template<typename C1> struct Dummy222;
struct Dummy333;
struct Dummy444;
struct Dummy555;
struct Dummy666;

template <class C1, class C2=Dummy000<C1::b> >
struct CP;

template <class C1=Dummy333, class C2=Dummy444>
struct N1 {
public:
  static const int b = 1;
};

template <class MatrixTypeTEMPLATE>
struct N2 {
  static const int b = 1;
};

template <class C1>
struct M {};

template<class C1>
struct MG {
public:
  void setProblem(C1 problem);
};

int main() {
  using T1 = M<CP<N1<Dummy111<int>, Dummy222<Dummy666>>>>;
  using T2 = M<CP<N2<Dummy555>>>;
  T2 p;
  MG<T1> m;
  m.setProblem(p);
