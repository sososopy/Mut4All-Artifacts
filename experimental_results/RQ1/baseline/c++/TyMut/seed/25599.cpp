
int A;
namespace Type {  struct A;  }

void f5() {
  using Type::A;
  using ::A;
}

