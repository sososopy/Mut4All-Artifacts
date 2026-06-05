
const char&
foo(const char* ptr)
{
  return ptr ? *ptr : throw ptr;
}
