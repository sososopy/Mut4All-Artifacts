
#include <utility>

void foo(){}

int main(){
    void(&f)() = foo;
    static_assert(!std::is_pointer<decltype(f)>::value, "should not be a pointer");
    return f != 0;
}
