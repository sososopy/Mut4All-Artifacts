
#include "Eigen/Core"

auto f() -> Eigen::Matrix2f {
  return Eigen::Matrix2f::Zero();
}

auto g() -> Eigen::Vector2f {
  auto v = Eigen::Product(f(), Eigen::Vector2f::Zero());
  return v;
}
