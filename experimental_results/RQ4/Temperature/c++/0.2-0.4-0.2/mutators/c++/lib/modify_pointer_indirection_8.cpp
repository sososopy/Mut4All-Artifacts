//source file
#include "../include/modify_pointer_indirection_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DRE = Result.Nodes.getNodeAs<clang::UnaryOperator>("PointerIndirection")) {
      //Filter nodes in header files
      if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DRE->getExprLoc()))
        return;

      //Get the source code text of target node
      auto expr = stringutils::rangetoStr(*(Result.SourceManager), DRE->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string nonPointerReplacement = "42"; // Replace with a non-pointer type, e.g., an integer
      expr.replace(expr.find("*"), 1, "*" + nonPointerReplacement);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DRE->getSourceRange()), expr);
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = unaryOperator(hasOperatorName("*")).bind("PointerIndirection");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}