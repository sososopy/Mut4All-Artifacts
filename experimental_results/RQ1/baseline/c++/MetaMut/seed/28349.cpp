

#include <cstring>
#include <array>

constexpr std::size_t ARRAY_LEN = 10u;

int main()
{
    int arr[ARRAY_LEN];
    std::memset(arr, 0, ARRAY_LEN); // OK, warning as expected
    
    std::array<int, ARRAY_LEN> arr2;
    std::memset(arr2.data(), 0, arr2.size()); // No warning; note that both std::array::data() and std::array::size() are constexpr

    return 0;
}
