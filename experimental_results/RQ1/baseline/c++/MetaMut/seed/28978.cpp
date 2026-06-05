
struct S1 {
  S1 () {}
};

template <typename T>
class S7 {
protected:
  T c[10];

public:
  S7 () {
#pragma omp task affinity (iterator (i = 0 : 10 : 1): c[i])
    for (int k = 0; k < 1; ++k)
      ;
  }
};

struct S8 : public S7<S1> {
  S8 () : S7<S1> () {}
};
