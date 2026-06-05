

extern int len (const char *__s) __attribute__ ((__nonnull__ (1)));
extern int num;

int
f (void)
{
  return len (num != 2 ? "a" : 0);
}

