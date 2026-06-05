
struct nodefault {
  constexpr nodefault(int) { }
};
constexpr nodefault x[1] = { nodefault{0} };
constexpr nodefault y = x[0];



