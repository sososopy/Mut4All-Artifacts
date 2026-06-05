
#include <utility>

struct X {
    X f() && {
        return std::move(*this);
    }
};
