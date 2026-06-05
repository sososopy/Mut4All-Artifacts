
#include <catch.hpp>

struct Type1 {};

struct Type2 {};

namespace Op {
    bool operator==(const Type1&, const Type2&);
}

//#define WorkAround
#ifndef WorkAround
using namespace Op;
#else
bool operator==(const Type1& t1, const Type2& t2) {
    return Op::operator==(t1, t2);
}
#endif

TEST_CASE("") {
    Type1 t1;
    Type2 t2;

    CHECK(t1 == t2);
}
