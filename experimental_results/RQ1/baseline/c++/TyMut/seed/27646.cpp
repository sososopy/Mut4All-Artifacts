
static inline unsigned short
swap (unsigned short x)
{
  return (x >> 8) | static_cast<unsigned short>(x << 8);
}
