
#include <cassert>
#include <cstring>

struct foo
{
    int member : 8;
    // We explitly introduce 24 padding bits here.
    int : 24;

    bool operator==(const foo&) const = default;
};

// Yet this assertion passes?!
static_assert(__has_unique_object_representations(foo));

// But we clearly don't have unique object representations:
int main()
{
    // Create two objects with different object representations.
    foo a, b;
    std::memset(&a, 0xFF, sizeof(foo));
    std::memset(&b, 0x99, sizeof(foo));

    // Make all members identical to give them the same value representation.
    a.member = 0;
    b.member = 0;
    // This passes; they have the same value representation.
    assert(a == b);

    // But this fails; they don't have unique object representations!
    assert(std::memcmp(&a, &b, sizeof(foo)) == 0);
}
