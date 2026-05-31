//source file
#include "../include/Alter_Template_Parameters_In_Lambda_Expressions_479.h"

// ========================================================================================================
#define MUT479_OUTPUT 1

void MutatorFrontendAction_479::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto &Context = *Result.Context;
      auto &SM = Context.getSourceManager();
      auto &LangOpts = Context.getLangOpts();
      auto &Rewrite = this->Rewrite;

      auto LambdaRange = LambdaExpr->getSourceRange();
      auto LambdaText = Lexer::getSourceText(CharSourceRange::getTokenRange(LambdaRange), SM, LangOpts).str();

      std::string newLambdaText = LambdaText;
      size_t templatePos = newLambdaText.find("<");
      if (templatePos != std::string::npos) {
          newLambdaText.insert(templatePos + 1, newConceptName + ", ");
      }

      size_t operatorPos = newLambdaText.find("operator()<");
      if (operatorPos != std::string::npos) {
          newLambdaText.insert(operatorPos + 11, "char, ");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), newLambdaText);
    }
}
  
void MutatorFrontendAction_479::MutatorASTConsumer_479::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}