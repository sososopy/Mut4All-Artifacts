
unsigned char
sr ()
{
  const unsigned char xz = EI;

  return xz - (xz >> 1);
}
