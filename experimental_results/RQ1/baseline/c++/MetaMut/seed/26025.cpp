// compile error with gcc 4.7 .. 4.9, clang < 3.3
// Option:  -std=c++11

#include <vector>

struct Pardef_s{};
struct LPardef_s: public Pardef_s
{
    LPardef_s(const char*, const char*){}
};

class Glob_c
{
public:
    const static std::vector<Pardef_s> Par;
};

const std::vector<Pardef_s> Glob_c::Par=
{
LPardef_s({
        "LInitState",
        "pInitState"
})};
