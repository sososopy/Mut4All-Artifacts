struct T {
  _Complex double z;
} v;

_Complex double
foo (_Complex double z)
{
  ++v.z;
  return 0;
}