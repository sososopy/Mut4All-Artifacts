//source file
#include "../include/Replace_Decltype_Auto_With_Auto_In_Var_Decl_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("DecltypeAutoVar")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto DLtype = DL->getTypeSourceInfo();
      auto DLtypestr = stringutils::rangetoStr(*(Result.SourceManager),
                                               DLtype->getTypeLoc().getSourceRange());
      llvm::outs() << DLtypestr << '\n';
      if (DLtypestr == "decltype(auto)") {
        DLtypestr = "auto";
        Rewrite.ReplaceText(DLtype->getTypeLoc().getSourceRange(), DLtypestr);
      }
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("DecltypeAutoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}