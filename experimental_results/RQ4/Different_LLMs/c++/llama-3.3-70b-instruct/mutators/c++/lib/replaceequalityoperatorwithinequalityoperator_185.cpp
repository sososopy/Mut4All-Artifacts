//source file
#include "../include/ReplaceEqualityOperatorWithInequalityOperator_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryExpr")) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getOperatorLoc()))
        return;
      //Check if the binary operator is an equality operator
      if (BE->getOpcode() == clang::BinaryOperatorKind::BO_EQ) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   BE->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        declaration.replace(declaration.find("=="), 2, "!=");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BE->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("==")).bind("BinaryExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}