//source file
#include "../include/incomplete_initializer_in_struct_or_union_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->hasInit()) {
        auto initExpr = VD->getInit();
        auto initRange = initExpr->getSourceRange();
        std::string initText = Rewrite.getRewrittenText(initRange);

        if (!initText.empty()) {
          std::string mutatedInit = initText.substr(0, initText.find('=') + 1) + ";";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(initRange), mutatedInit);
        }
      }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(anything())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}