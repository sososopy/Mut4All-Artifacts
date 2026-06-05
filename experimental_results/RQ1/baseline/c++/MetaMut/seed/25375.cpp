
extern "Java"
{
  typedef __java_int jint;
  namespace java
  {
    namespace lang
    {
      class Class;
      class Object;
      class Throwable {};
      class Foo;
    }
  }
} 

typedef struct java::lang::Object * jobject;
typedef struct java::lang::Throwable * jthrowable;
typedef class  java::lang::Class * jclass;

using java::lang::Foo;

class Foo: public java::lang::Throwable
{
  public:static::java::lang::Class class$;
};

extern "C" Foo _Jv_AllocObject (jclass) __attribute__ ((__malloc__));
extern "C" void _Jv_Throw (jthrowable) __attribute__ ((__noreturn__));

void 
Bar4 (void)
{
  Foo * f = new java::lang::Foo;
  throw (f);
}
