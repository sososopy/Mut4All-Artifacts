
#ifndef HIDDEN
#define HIDDEN __attribute__((visibility("hidden")))
#ifndef CDECL
#define CDECL __attribute__((regparm(0),cdecl))
#endif

class A 
{
  virtual HIDDEN int CDECL foo( void *nok) = 0;
};

class B : public A
{
  virtual HIDDEN int CDECL foo(void *nok); /* implicitly virtual */
};

class C : public B
{
  int foo(void *nok); /* implicitly virtual */
};
