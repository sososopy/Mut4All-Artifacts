
struct T {
  auto f() & {
    return int{0};
  }
  auto f() && {
    return string{""};
  }
};

void test_it() {
  //Calling with L-value object. Fails with "call of overloaded 'f()' is ambiguous").
  T t;
  int s = t.f();

  //Calling with R-value object. Fails with "call of overloaded 'f()' is ambiguous").
  string i = T{}.f();
}

struct T2 {
  auto f() & -> int {
    return 0;
  }
  auto f() && -> string {
    return "";
  }
};

void test_it_2() {
  //Calling with L-value object. Works just fine when the return type is stated!
  T2 t;
  int s = t.f();
  
  //Calling with R-value object. Works just fine when the return type is stated!
  string i = T2{}.f();
}

struct T3 {
  auto f() & {
    return 0;
  }
  auto f() && -> string {
    return "";
  }
};

void test_it_3() {
  //Calling with L-value object. Works fine when the non-selected overload has a non-deduced return type(!).
  T3 t;
  int s = t.f();
  
  //Calling with R-value object. Doesn't work even though the selected overload has a non-deduced return type.
  string i = T3{}.f();
}

