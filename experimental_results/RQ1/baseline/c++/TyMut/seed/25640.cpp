
struct foo { };
  typedef struct
  {
      volatile foo fields;
  }
  CSPHandleState;
  CSPHandleState a;
  void
  fn1 ()
  {
      CSPHandleState b;
      b.fields = foo();
  }

