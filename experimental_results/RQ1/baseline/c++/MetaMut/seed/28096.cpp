
struct Test {
    std::byte a : 2 = std::byte{0}; // NOK
    uint8_t   b : 2 = 0; // OK
};

