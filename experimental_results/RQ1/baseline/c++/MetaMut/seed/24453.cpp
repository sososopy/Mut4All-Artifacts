constexpr int foo(float f) {return int(f);} 

int main() 
{ 
    const float x = 0.5; 
    static_assert(x > 0.1, "all good"); // #1
    constexpr int i = foo(x); // #2
}
