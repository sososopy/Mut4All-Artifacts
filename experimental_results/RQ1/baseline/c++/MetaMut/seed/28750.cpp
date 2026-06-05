
//exp.cpp
module;

#include <string>

export module mod;

export std::string F() { return "test"; }
//imp.cpp
#include <span>
#include <string>
#include <cstdint>

import mod;

constexpr void
ToHexInt(uint8_t value, char& dst) {
    if (value <= 9)
	dst = static_cast<char>('0' + value);
    if (value >= 0xa && value <= 0xf)
	dst = static_cast<char>('a' + (value - 0x0a));
}

template <class DestT = std::string>
inline DestT
ToHexString(const std::span<const uint8_t>& value) {
    DestT output;
    output.resize(value.size() * 2);
    uint8_t i;
    ToHexInt(i, output[0]);
    return output;
}

int main() {
    uint8_t a[2];
    ToHexString(a);
}
