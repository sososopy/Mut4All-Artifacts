
#include <string>

using F = std::string(*)(std::string);

std::string call_fn_pointer(F f, std::string s)
{
    return f(s);
}

std::string f(std::string s) { return s; }

int main()
{
    // fine
    call_fn_pointer(f, "foo");

    // fine
    call_fn_pointer([] (std::string s) { return std::string(s); }, "foo");

    // double free
    call_fn_pointer([] (std::string s) { return s; }, "foo");
}

