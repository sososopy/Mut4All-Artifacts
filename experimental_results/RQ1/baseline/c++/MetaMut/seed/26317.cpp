
#include <stdint.h>
#include <stddef.h>

void
xorblock(uint8_t* dest, const uint8_t* src, size_t len)
{
  while (len--)
    *dest++ ^= *src++;
}

