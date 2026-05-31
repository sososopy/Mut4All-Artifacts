//source file
#include "../include/Modify_Operator_In_Template_Function_Return_326.h"

// ========================================================================================================
#define MUT326_OUTPUT 1

void MutatorFrontendAction_326::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>("templateReturnOperator")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getOperatorLoc()))
        return;
      auto op = MT->getOpcodeStr();
      string op_str = string(op);
      llvm::outs() << op_str << '\n';
      if (op_str == "+" || op_str == "-")
        op_str = "*";
      else if (op_str == "*")
        op_str = "-";
      else if (op_str == "<" || op_str == ">")
        op_str = "-";
      else if (op_str == "==")
        op_str = "-";
      else if (op_str == "-")
        op_str = "+";
      Rewrite.ReplaceText(MT->getOperatorLoc(), op.size(), op_str);
    }
}
  
void MutatorFrontendAction_326::MutatorASTConsumer_326::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(
                       isTemplateInstantiation(),
                       hasDescendant(returnStmt(has(binaryOperator().bind("templateReturnOperator")))))
                       .bind("templateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}