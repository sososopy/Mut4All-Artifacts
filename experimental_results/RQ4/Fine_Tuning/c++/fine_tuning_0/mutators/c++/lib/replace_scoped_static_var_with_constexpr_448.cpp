//source file
#include "../include/Replace_Scoped_Static_Var_With_Constexpr_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("scopedStaticVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getBeginLoc()))
        return;
      if (!VD->isStaticLocal())
        return;
      auto type = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getTypeSourceInfo()->getTypeLoc().getSourceRange());
      auto name = VD->getNameAsString();
      auto init = stringutils::rangetoStr(*(Result.SourceManager),
                                          VD->getInit()->getSourceRange());
      string new_decl = "constexpr " + type + " " + name + " = " + init;
      llvm::outs() << new_decl << '\n';
      Rewrite.ReplaceText(VD->getSourceRange(), new_decl);
    }
}
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasAncestor(functionDecl())).bind("scopedStaticVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}