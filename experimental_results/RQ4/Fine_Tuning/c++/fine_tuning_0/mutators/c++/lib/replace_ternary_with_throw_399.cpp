//source file
#include "../include/Replace_Ternary_With_Throw_399.h"

// ========================================================================================================
#define MUT399_OUTPUT 1

void MutatorFrontendAction_399::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConditionalOperator>("Ternary")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto ternaryExpr = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string throwExpr = "throw 0";
      llvm::outs() << "/*mut399*/" << throwExpr << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), throwExpr);
    }
}
  
void MutatorFrontendAction_399::MutatorASTConsumer_399::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = conditionalOperator().bind("Ternary");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}