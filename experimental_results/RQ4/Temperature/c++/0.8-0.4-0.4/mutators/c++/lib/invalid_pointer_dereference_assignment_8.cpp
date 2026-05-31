//source file
#include "../include/invalid_pointer_dereference_assignment_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->hasInit()) {
        auto *InitExpr = VD->getInit();
        if (isa<IntegerLiteral>(InitExpr)) {
          auto InitRange = InitExpr->getSourceRange();
          std::string MutatedInit = "*" + InitExpr->getSourceRange().getBegin().printToString(Result.Context->getSourceManager());
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitRange), MutatedInit);
        }
      }
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(integerLiteral())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}