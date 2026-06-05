
#include <array>

template <typename T, typename Ix, Ix Size>
class Pen : std::array<T, Size> {
  typedef std::array<T, Size> arr;
public:
  // Removing either "constexpr" or ": arr()" from the following
  // line seems to work around the problem.
  constexpr Pen() : arr() { }
};

class Farm {
public:
  enum Sheep { BAA, ZZZ };

  template<typename T>
  using SheepPen = Pen<T, Sheep, ZZZ>;
};

class Fence {
public:
  constexpr Fence() { length = 12; }
  int length = 0;
};

void
cull() {
  static Farm::SheepPen<Fence> s;
}

