//source file
#include "../include/Mutator_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("outerLambda")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      
      // Check if this lambda is generic (has template parameter list)
      auto *Method = LE->getCallOperator();
      if (!Method || !Method->getDescribedFunctionTemplate())
        return;
      
      // Store the lambda for later processing
      outerLambdas.push_back(LE);
    }
}

void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    // Implement mutation logic here
}