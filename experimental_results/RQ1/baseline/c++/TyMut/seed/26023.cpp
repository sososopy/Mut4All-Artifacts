
struct Base {
  virtual ~Base();
};

extern "C" int printf(const char *fmt, ...);

Base::~Base() {
  printf("OK: %s\n", __FUNCTION__);
}

void call_complete_dtor(Base* obj) {
  obj->~Base();
  printf("After obj->~Base();\n");
}
