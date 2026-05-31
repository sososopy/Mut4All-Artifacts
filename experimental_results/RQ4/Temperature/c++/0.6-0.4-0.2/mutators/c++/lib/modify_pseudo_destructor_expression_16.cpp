//source file
#include "../include/modify_pseudo_destructor_expression_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("PseudoDestructorExpr")) {
      // Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;
      
      // Get the source code text of target node
      auto exprSourceRange = MT->getSourceRange();
      auto exprSourceText = stringutils::rangetoStr(*(Result.SourceManager), exprSourceRange);

      // Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = "(*ptr).~NonExistentType()";
      if (exprSourceText.find("~") != std::string::npos) {
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(exprSourceRange, mutatedExpr);
      }
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxMemberCallExpr(
                      callee(cxxMethodDecl(hasName("~PseudoDestructorExpr")))
                   ).bind("PseudoDestructorExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}