

struct VectorType {
  void operator=(VectorType);
};
template <typename> using V4 = VectorType;
using Float = V4<float>;
struct vec3 {
  Float x;
};
struct mat3 {
  vec3 data[3];
  constexpr mat3() { data[2] = vec3(); }
};

