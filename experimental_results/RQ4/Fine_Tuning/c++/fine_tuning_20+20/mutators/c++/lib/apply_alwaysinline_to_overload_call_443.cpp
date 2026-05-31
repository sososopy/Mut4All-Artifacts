//source file
#include "../include/apply_alwaysinline_to_overload_call_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Overloads")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getOverloadedOperator() != OverloadedOperatorKind::OO_None)
        return;
      overloads.push_back(FD);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      if (overloads.empty())
        return;
      auto callee = CL->getDirectCallee();
      if (!callee)
        return;
      if (callee->getOverloadedOperator() != OverloadedOperatorKind::OO_None)
        return;
      if (callee->getPrimaryTemplate() == nullptr)
        return;
      auto callname = callee->getNameAsString();
      for (auto overload : overloads) {
        if (overload->getNameAsString() == callname) {
          Rewrite.ReplaceText(CL->getBeginLoc(), 0, "[[clang::always_inline]] ");
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto overload_matcher = functionDecl().bind("Overloads");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(overload_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}