//source file
#include "../include/Modify_Lambda_Expression_With_Dependent_Type_121.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t paramPos = lambdaText.find(">");
      if (paramPos != std::string::npos) {
          lambdaText.insert(paramPos, ", typename U");
      }

      size_t noexceptPos = lambdaText.find("noexcept");
      if (noexceptPos != std::string::npos) {
          size_t requiresPos = lambdaText.find("requires", noexceptPos);
          if (requiresPos != std::string::npos) {
              lambdaText.insert(requiresPos + 8, " requires { typename U::dependentType; }");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}