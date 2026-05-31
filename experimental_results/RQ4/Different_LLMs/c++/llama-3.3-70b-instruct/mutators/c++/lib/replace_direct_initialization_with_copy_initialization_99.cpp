//source file
#include "../include/Mutator_Replace_Direct_Initialization_With_Copy_Initialization_99.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      size_t pos = declaration.find('(');
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, 1, " = ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}