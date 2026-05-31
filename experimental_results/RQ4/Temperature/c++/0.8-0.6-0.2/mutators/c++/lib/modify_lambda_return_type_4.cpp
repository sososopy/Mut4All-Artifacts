//source file
#include "../include/modify_lambda_return_type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLambdaClass()->getLocation()))
        return;

      auto *D = LE->getLambdaClass()->getLambdaCallOperator();
      if (!D)
        return;

      //Get the source code text of target node
      auto originalSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    D->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedSource = originalSource;
      std::string returnType = "int (int)";
      size_t pos = originalSource.find("->");
      if (pos != std::string::npos) {
          auto endPos = originalSource.find('{', pos);
          mutatedSource.replace(pos, endPos - pos, "-> " + returnType);
      } else {
          size_t lambdaPos = originalSource.find('{');
          if (lambdaPos != std::string::npos) {
              mutatedSource.insert(lambdaPos, " -> " + returnType);
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(D->getSourceRange()), mutatedSource);
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}