//source file
#include "../include/modify_template_parameter_packs_and_concepts_631.h"

// ========================================================================================================
#define MUT631_OUTPUT 1

void MutatorFrontendAction_631::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;
      
      auto lambdaRange = LambdaExpr->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

      std::string mutatedLambdaText = lambdaText;
      size_t paramListStart = mutatedLambdaText.find('(');
      size_t paramListEnd = mutatedLambdaText.find(')', paramListStart);

      if (paramListStart != std::string::npos && paramListEnd != std::string::npos) {
          std::string paramList = mutatedLambdaText.substr(paramListStart + 1, paramListEnd - paramListStart - 1);
          mutatedLambdaText.insert(paramListEnd, ", " + paramList);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedLambdaText);
    }
}
  
void MutatorFrontendAction_631::MutatorASTConsumer_631::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}