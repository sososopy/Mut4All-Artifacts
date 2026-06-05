

constexpr bool static_str_equal(const char* x, const char* y) {
   return (*x == 0 || *y == 0) ?
     (*x == *y) :
     ((*x == *y) && static_str_equal(x + 1, y + 1));
}
int main(void)
{
  static_assert( !static_str_equal("unspecified1", "unspecified"), "");
}

