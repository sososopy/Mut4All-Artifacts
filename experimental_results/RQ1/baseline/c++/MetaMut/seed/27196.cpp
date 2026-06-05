
struct A { long x; };

union U;
constexpr A foo(U *up);

union U {
  A a = foo(this); int y;
};

constexpr A foo(U *up) {
  up->y = 11;
  return {42};
}

extern constexpr U u = {};

