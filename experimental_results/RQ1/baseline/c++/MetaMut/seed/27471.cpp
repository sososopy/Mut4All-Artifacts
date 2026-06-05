
#include <iostream>

struct Float {
    float value;

    constexpr Float(float value) : value (value) {}

    constexpr bool operator == (Float other) const noexcept {
        return abs(value - other.value) < 0.5;
    }

    constexpr bool operator != (Float other) const noexcept {
        return !(*this == other);
    }

    constexpr bool operator <=> (const Float& other) const noexcept = default;
};

