//source file
#include "../include/Replace_Decltype_Auto_With_Auto_For_Function_Template_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isFunctionPointerType() == false)
        return;
      auto DLtype = DL->getType();
      auto DLtypestr = DLtype.getAsString();
      llvm::outs() << DLtypestr << '\n';
      if (DLtypestr.find("decltype") == std::string::npos)
        return;
      DLtypestr.replace(DLtypestr.find("decltype(auto)"), 14, "auto");
      llvm::outs() << DLtypestr << '\n';
      Rewrite.ReplaceText(DL->getBeginLoc(), 14, "auto");
    }
}
  
void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = declRefExpr().bind("DeclRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}