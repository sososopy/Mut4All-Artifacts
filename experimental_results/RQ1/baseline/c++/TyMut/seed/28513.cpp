
#include <iostream>
#include <vector>
#include <numeric>

int main(int /*argc*/, char* /*argv*/[]) {

  const std::size_t val_size_t = 1000;

  std::vector<double> vals(val_size_t, 1.0);

  std::iota(vals.begin(), vals.end(), 0.0);

  auto squared = std::accumulate(vals.begin(), vals.end(), 0.0,
                             [](const double& tot, const double& vA) {
                               return tot + vA * vA;
                             });
  std::cout << "Squared: " << squared << std::endl;

  double squared2 = 0.0;
  for (const auto& v : vals) {
    squared2 += v * v;
  }
  std::cout << "Squared 2: " << squared2 << std::endl;

  return 0;

}

