
struct S {
      int m;
      static void f_sizeof() { (void) sizeof(m); }
      static void f_typeid() { (void) typeid(decltype(m)); }
 
      struct N {
      int m;
      static void f_sizeof() { (void) sizeof(m); }
      static void f_typeid() { (void) typeid(decltype(m)); }
      };
 
      template <class T> class C {
      public:
      int m;
      static void f_sizeof() { (void) sizeof(m); }
      static void f_typeid() { (void) typeid(decltype(m)); }
      };
  } s;
static void f_typeid() { (void) typeid(m); }

