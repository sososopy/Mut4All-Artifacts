
typedef const long long * ppp;
// typedef const int * ppp;

__m128i VPGATHERDQ1(ppp base, __m128i index) {
   return _mm_i32gather_epi64 (base, index, 1);
}

__m256i VPGATHERDQ2 (ppp base, __m128i index) {
   return _mm256_i32gather_epi64 (base, index, 2);
}

__m128i VPGATHERQQ1(ppp base, __m128i index) {
   return _mm_i64gather_epi64 (base, index, 4);
}

__m128i VPGATHERQQ2(ppp base, __m128i index) {
   return _mm256_i64gather_epi64 ( base,  index, 8);
}
