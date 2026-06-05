
#include <unistd.h>

void fn (int *data) {
  write (1, data++, sizeof (*data));
}

