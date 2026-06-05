
#include <vector>
#include <string.h>
#include <stdlib.h>

struct z {
  char* var;
  z (const char* str) { var = strdup (str);  }
  ~z () { free (var); }
};

std::vector<z> y;

int main () {
  y.emplace_back ("a");
  y.emplace_back ("b");
}

