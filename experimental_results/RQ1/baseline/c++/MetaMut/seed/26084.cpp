#include <assert.h>
int main() {
    const char* buf = "12345678";
    const int& cref = (const int&)buf[0];
    const int&  ref = (      int&)buf[0];
    assert(cref == ref);
    return 0;
}
