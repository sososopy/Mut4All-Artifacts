
#include <type_traits>

int main()
{
   static_assert( std::is_trivially_copyable_v<         int[5]>);
   static_assert( std::is_trivially_copyable_v<volatile int[5]>);  // valid?
}

