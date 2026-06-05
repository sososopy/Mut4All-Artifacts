
short a = 0;
long const b = 0;
enum class c {e}; 
template <typename = enum class c { d = b || a}> class A{}; 
template <> enum class c {};

