
#define OOPS

struct Class
{
  Class(int func)
  try
#ifdef OOPS
  : f{ func }  { }
#else
  : f( func )  { }
#endif
  catch( ... ) { }

private:
  int f;
};
