
#pragma GCC target "avx"
template <typename> struct Simd {};
#pragma GCC push_options
#pragma GCC target "avx,avx2,bmi,bmi2,fma,f16c"
template <typename T> using Full256 = Simd<T>;
template <typename> struct BitCastFromInteger256;
template <> struct BitCastFromInteger256<float> {
  __attribute__((always_inline)) float operator()(long) { return .0f; }
};
long BitCastFromByte_v_0;
template <typename T> void BitCastFromByte(Full256<T>) {
  T{BitCastFromInteger256<T>()(BitCastFromByte_v_0)};
}
template <typename T, typename FromT> void BitCast(T d, FromT) {
  BitCastFromByte(d);
}
int EstimateEntropy___trans_tmp_3;
void EstimateEntropy() {
  Simd<float> df;
  BitCast(df, EstimateEntropy___trans_tmp_3);
}
#pragma GCC pop_options

