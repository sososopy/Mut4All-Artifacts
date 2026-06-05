
void Func() {
  struct StructDeclaredInsideFunc {
    // This is OK.
    auto Bar() -> int {
      return 1;
    }
  };
}

class Foo {
  public:
  void Func();
};

void Foo::Func() {
  struct StructDeclaredInsideClassFunc {
    // Causes:
    // g++4.7 (Debian 4.7.2-4): internal compiler error: in cp_parser_late_return_type_opt, at
    // cp/parser.c:16698
    // g++ Snapshot (4.8.0 20130316): internal compiler error: in cp_parser_late_return_type_opt, at
    // cp/parser.c:16970
    auto Bar() -> int {
      return 1;
    }
  };
}
