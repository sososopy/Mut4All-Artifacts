//source file
#include "../include/static_variable_incrementation_in_consteval_325.h"

// ========================================================================================================
#define MUT325_OUTPUT 1

void MutatorFrontendAction_325::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SVD = Result.Nodes.getNodeAs<clang::VarDecl>("staticVarDecl")) {
      //Filter nodes in header files
      if (!SVD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SVD->getLocation()))
        return;

      if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("incrementOp")) {
        //Get the source code text of target node
        auto incrementExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                                     BO->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedExpr = "1 / (" + incrementExpr + " - " + incrementExpr + ")";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedExpr);
      }
    }
}
  
void MutatorFrontendAction_325::MutatorASTConsumer_325::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher staticVarMatcher = varDecl(hasStaticStorageDuration(),
                                                  hasAncestor(functionDecl(isConstexpr())))
                                                  .bind("staticVarDecl");
    StatementMatcher incrementMatcher = binaryOperator(hasOperatorName("++"),
                                                       hasLHS(declRefExpr(to(varDecl(hasStaticStorageDuration())))))
                                                       .bind("incrementOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(staticVarMatcher, &callback);
    matchFinder.addMatcher(incrementMatcher, &callback);
    matchFinder.matchAST(Context);
}