
typedef float float2 __attribute__((ext_vector_type(2)));
typedef float float4 __attribute__((ext_vector_type(4)));

void prepareWarpLinear(const float2 in, const float2 out) {
  const float4 srcRay = (float4){ in, out };
}
