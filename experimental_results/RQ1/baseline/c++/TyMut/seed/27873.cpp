
static const char *const rtx_format[1] = { "" };

int main(int argc, char **argv)
{
  if (rtx_format[0] == 0)
    __builtin_abort ();
  return 0;
}
