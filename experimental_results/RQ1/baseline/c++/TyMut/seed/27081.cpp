

typedef char * CP;

const char * foo (void)
{
  return ((const CP)"abc"+1);
}

