
#include <stdint.h>
#include <iostream>

struct BitsOrderCheck
{
    union Data
    {
        struct Bitfield
        {
            const unsigned char   clear:7;
            const unsigned char   set:1;
        };

        const unsigned char   byte;
        const Bitfield        bits;
        constexpr Data() : byte(1) {}
    };
    constexpr BitsOrderCheck() {}
    constexpr bool IsLsbBottom() const
    {
        return 1 == data_.bits.set;
    }

    const Data    data_;
};

int main()
{
    static_assert(BitsOrderCheck().IsLsbBottom(), "blah");
    std::cout << "LSB: " << BitsOrderCheck().IsLsbBottom() << std::endl;
    return 0;
}
