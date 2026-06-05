

#include <array>
#include <vector>

int main() {
  std::vector<std::array<int, 20>> out;

  auto saveChan = [&](auto && f) {
        f({});
  };
  saveChan([](decltype(out)::value_type const & i){ return i;});
}
