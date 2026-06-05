
#include <cstdio>
#include <cstring>
#include <new>

struct struct_with_padding
{
    char x;
    short y;
};

template< typename To, typename From >
inline To bitwise_cast_clear_padding(From const& from) noexcept
{
    union cast_storage_t
    {
        unsigned char bytes[sizeof(To) < sizeof(From) ? sizeof(From) : sizeof(To)];
        From aligner;

        cast_storage_t() {}
    }
    storage;

    // Clear any possible internal padding bits. Placement new is required to perform member-wise initialization.
    std::memset(storage.bytes, 0, sizeof(storage.bytes));
    From* p = new (storage.bytes) From(from);

    To to;
    std::memcpy(&to, p, sizeof(to));

    return to;
}

int main()
{
    struct_with_padding a;
    std::memset(&a, 0xCC, sizeof(a));
    a.x = 1;
    a.y = 1;

    std::printf("%08x\n", bitwise_cast_clear_padding< unsigned int >(a));
}
