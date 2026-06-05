
#include <cstdint>

uint16_t samplesBuffer[40];

template <typename T>
constexpr inline auto getNumberOfSamples()
{
    // No matter how much parentheses are added, warnings still persists
    return ((sizeof(samplesBuffer)) / (sizeof(T))); 
}

int main()
{
    // return sizeof(samplesBuffer) / (sizeof(uint8_t)); // Works fine, no warning
    return getNumberOfSamples<uint8_t>(); // Results in warning, how to silence it?
}
