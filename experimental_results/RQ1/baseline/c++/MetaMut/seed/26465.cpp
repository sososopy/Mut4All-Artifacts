
#include <array>
#include <bit>
#include <cstdint>

using uint64_x_2_t = std::array<std::uint64_t, 2>;
using uint32_x_2_t = std::array<std::uint32_t, 2>;
using uint16_x_2_t = std::array<std::uint16_t, 2>;

static_assert(sizeof(long double) == sizeof(uint64_x_2_t));
static_assert(sizeof(double) == sizeof(uint32_x_2_t));
static_assert(sizeof(float) == sizeof(uint16_x_2_t));

constexpr long double testld = 42.42;
constexpr double testd = 42.42;
constexpr float testf = 42.42f;

constexpr uint64_x_2_t test_uint64_x_2_t{1u, 2u};
constexpr uint32_x_2_t test_uint32_x_2_t{1u, 2u};
constexpr uint16_x_2_t test_uint16_x_2_t{1u, 2u};

constexpr auto ld_to_uint64_x_2_t = std::bit_cast<uint64_x_2_t>(testld); // works! (reverse direction)
constexpr auto d_to_uint32_x_2_t = std::bit_cast<uint32_x_2_t>(testd);
constexpr auto f_to_uint16_x_2_t = std::bit_cast<uint16_x_2_t>(testf);

constexpr auto default_uint64_x_2_t_to_ld = std::bit_cast<long double>(uint64_x_2_t{}); // works! (default initialized)
constexpr auto default_uint32_x_2_t_to_d = std::bit_cast<double>(uint32_x_2_t{});
constexpr auto default_uint16_x_2_t_to_f = std::bit_cast<float>(uint16_x_2_t{});

constexpr auto temp_uint64_x_2_t_to_ld = std::bit_cast<long double>(uint64_x_2_t{1u, 2u}); // <= fails
constexpr auto temp_uint32_x_2_t_to_d = std::bit_cast<double>(uint32_x_2_t{1u, 2u});
constexpr auto temp_uint16_x_2_t_to_f = std::bit_cast<float>(uint16_x_2_t{1u, 2u});

constexpr auto uint64_x_2_t_to_ld = std::bit_cast<long double>(test_uint64_x_2_t); // <= fails
constexpr auto uint32_x_2_t_to_d = std::bit_cast<double>(test_uint32_x_2_t);
constexpr auto uint16_x_2_t_to_f = std::bit_cast<float>(test_uint16_x_2_t);
