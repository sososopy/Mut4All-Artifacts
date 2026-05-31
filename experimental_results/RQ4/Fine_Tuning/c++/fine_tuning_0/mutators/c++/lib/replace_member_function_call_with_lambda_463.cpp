//source file
#include "../include/Replace_Member_Function_Call_With_Lambda_463.h"

// ========================================================================================================
#define MUT463_OUTPUT 1

void MutatorFrontendAction_463::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto call_expr = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      llvm::outs() << call_expr << '\n';
      auto callee = MT->getCalleeDecl();
      if (callee == nullptr)
        return;
      if (callee->getAccess() == AccessSpecifier::AS_private)
        return;
      auto result_type = MT->getType().getAsString();
      std::string lambda_expr = "[&]()";
      if (result_type != "void") {
        lambda_expr = "auto lambda = " + lambda_expr;
        call_expr = "lambda()";
      }
      lambda_expr += "{ return " + call_expr + "; }";
      Rewrite.ReplaceText(MT->getSourceRange(), lambda_expr);
    }
}
  
void MutatorFrontendAction_463::MutatorASTConsumer_463::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr().bind("MemberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}