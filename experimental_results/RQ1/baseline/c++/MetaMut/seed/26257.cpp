
#include <limits>
#include <cstdio>
#include <cstring>

int main()
{
  union {
    unsigned char b[sizeof(double)];
    double d;
    float f;
  } u;
  memset(&u, 0, sizeof(u));
  u.f = std::numeric_limits<float>::signaling_NaN(); //__builtin_nansf("");
  printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
         u.b[0], u.b[1], u.b[2], u.b[3],
         u.b[4], u.b[5], u.b[6], u.b[7]);
  u.f = std::numeric_limits<float>::quiet_NaN(); //__builtin_nanf("");
  printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
         u.b[0], u.b[1], u.b[2], u.b[3],
         u.b[4], u.b[5], u.b[6], u.b[7]);
  u.d = std::numeric_limits<double>::signaling_NaN(); //__builtin_nans("");
  printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
         u.b[0], u.b[1], u.b[2], u.b[3],
         u.b[4], u.b[5], u.b[6], u.b[7]);
  u.d = std::numeric_limits<double>::quiet_NaN(); //__builtin_nan("");
  printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
         u.b[0], u.b[1], u.b[2], u.b[3],
         u.b[4], u.b[5], u.b[6], u.b[7]);
}

