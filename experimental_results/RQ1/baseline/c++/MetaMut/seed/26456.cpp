
template <typename Type>
inline constexpr Type   my_sqrt(const Type x)   noexcept
{
    if (std::isless(x, 0.0)) __builtin_unreachable();
    return std::sqrt(x);
}

