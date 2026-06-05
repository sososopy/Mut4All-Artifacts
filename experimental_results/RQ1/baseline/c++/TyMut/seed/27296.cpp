
#include <concepts>
#include <ranges>
#include <vector>

auto transform_v = [] (auto && v)
{
    static_assert(std::same_as<decltype(v), int &>);
    return v;
};

using range_t = decltype(std::views::single(0));  // falsely calls const qualified begin somewhere in stack
// using range_t = std::vector<int>; // OK, everyting as expected.
using transformed_view_t = decltype(std::declval<range_t &>() | std::views::transform(transform_v));
using ref_t = std::ranges::range_reference_t<transformed_view_t>;
