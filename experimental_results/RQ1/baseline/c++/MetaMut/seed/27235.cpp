
#include <array>

int main() {
  struct nontriv { nontriv() { } };
  std::array<nontriv,100000> array = {}; // <- aggregate init
}
