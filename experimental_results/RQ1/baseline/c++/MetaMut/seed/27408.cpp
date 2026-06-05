
#include <set>

struct xx {
    int a;
    int operator<=>(const xx&) const = default;
};

int f(std::set<xx> &x) {
    x.emplace(0);
    return 0;
}
