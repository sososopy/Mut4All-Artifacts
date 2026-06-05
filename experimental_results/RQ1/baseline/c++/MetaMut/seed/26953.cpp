
class Visitor {
  void visitWinDeclSpec();
  typedef void (Visitor::*visitor_fun_ptr)(int);
  static visitor_fun_ptr b[];
};
Visitor::visitor_fun_ptr Visitor::b[]{
    visitor_fun_ptr(&Visitor::visitWinDeclSpec)};

