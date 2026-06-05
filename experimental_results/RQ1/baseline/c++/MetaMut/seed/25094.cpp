
#include <vector>

template<bool VAR>
struct Obj {
  std::vector<int> member;
  Obj() { }
  Obj(Obj&&) = default;
};

extern template struct Obj<true>;

int main() {
  Obj<true> o1;
  Obj<true> o2(std::move(o1));
}

template struct Obj<true>;


