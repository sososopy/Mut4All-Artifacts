
#include <stdint.h>
static void ff(uint64_t b) { [&](){}(); }
int main() { ff(0); }
