
#include <string>
#include <vector>

struct Info {
    std::vector<std::string> args;
    size_t arity = 0;
};

struct RegisterPrimOp
{
    RegisterPrimOp(Info && info) __attribute__((noipa, noinline)) {
        if (info.arity != 0)
            __builtin_trap();
    }
};

static RegisterPrimOp s_op({
    .args = {"path"},
    .arity = 0,
});

int main() {}

