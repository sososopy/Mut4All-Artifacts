
class Class1 {
 public:
  auto member() -> int&
#ifdef INLINE
  {
    return member_;
  }
#else
    ;
#endif
 private:
  int member_;
};

#ifndef INLINE
auto Class1::member() -> int& { return member_; }
#endif

extern Class1 obj1;
extern Class1 obj2;

int main() {
  auto value = obj1.member();
  auto value2 = obj2.member();

  return value + value2;
}

