//source file
#include "../include/replace_operator_new_with_custom_allocator_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      //Filter nodes in header files
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto newExprText = stringutils::rangetoStr(*(Result.SourceManager), NE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string customAllocator = "CustomAllocator::allocate";
      std::string mutatedExpr = newExprText;
      size_t pos = mutatedExpr.find("new");
      if (pos != std::string::npos) {
          mutatedExpr.replace(pos, 3, customAllocator);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()), mutatedExpr);
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}