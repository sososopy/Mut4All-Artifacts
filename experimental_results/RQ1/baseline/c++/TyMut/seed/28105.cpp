
#include <type_traits>

int main()
{
   static_assert( std::is_constructible_v<int[]>);
   static_assert( std::is_default_constructible_v<int[]>);            // fail

   static_assert( std:: is_trivially_constructible_v <int[]>);
   static_assert( std::is_trivially_default_constructible_v<int[]>);  // pass?!
}

