
module;

#include <cstddef>
#include <iostream>

export module VF.Tensor;

namespace VF
{
export
template<std::size_t d, std::size_t r1>
struct TTensor
{
private:
  std::istream
  inline &Read(std::istream &is)
    { return is; }
};

export
template<std::size_t d>
using TVector = TTensor<d, 1u>;
} // VF




module;

#include <cstddef>
#include <type_traits>

export module VF.Mesh;

import VF.Tensor;

namespace VF
{
template <std::size_t d>
class TPoint final : public TVector<d> {};
}

