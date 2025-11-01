//source file
#include "../include/Nest_Lambda_With_Parameter_Pack_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithPack")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        size_t lambdaStart = declaration.find("[&](");
        if (lambdaStart != std::string::npos) {
          size_t lambdaBodyStart = declaration.find("{", lambdaStart);
          if (lambdaBodyStart != std::string::npos) {
            std::string nestedLambda = "\n[&](auto... nestedIndexes) {";
            nestedLambda += "\n/*mut490*/";
            nestedLambda += declaration.substr(lambdaBodyStart + 1, declaration.find("}", lambdaBodyStart) - lambdaBodyStart - 1);
            nestedLambda += "\n};";
            declaration.insert(lambdaBodyStart + 1, nestedLambda);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_490::MutatorASTConsumer_490::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(hasDescendant(lambdaExpr())).bind("FunctionWithPack");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}