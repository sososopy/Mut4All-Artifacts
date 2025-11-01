//source file
#include "../include/Modify_Lambda_Template_Parameter_64.h"

// ========================================================================================================
#define MUT64_OUTPUT 1

void MutatorFrontendAction_64::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = Lexer::getSourceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), 
                                               *Result.SourceManager, 
                                               Result.Context->getLangOpts());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedLambda = lambdaSource.str();
      size_t pos = mutatedLambda.find("[]<");
      if (pos != std::string::npos) {
          mutatedLambda.insert(pos + 3, "auto Parameter, ");
          size_t bodyPos = mutatedLambda.find("{", pos);
          if (bodyPos != std::string::npos) {
              mutatedLambda.insert(bodyPos + 1, " static_assert(Parameter); ");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), mutatedLambda);
    }
}
  
void MutatorFrontendAction_64::MutatorASTConsumer_64::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}