
#pragma GCC optimize("finite-math-only")
#pragma GCC target("sse3")

void fn() {
}

int global_var;

int solve() {
    auto nested = []() {
        return global_var;
    };
    return nested();
}

