
#include <vector>

using func = void(*)();

void test() {}

int main() {
   std::vector<func> v = {&test};

   return 0;
}

