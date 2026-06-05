
typedef float __m256 __attribute__ (( __vector_size__(32), __may_alias__ ));
struct A {
  __m256 ymm;
  const float &f() const {
        return ymm[1];
  }
};


