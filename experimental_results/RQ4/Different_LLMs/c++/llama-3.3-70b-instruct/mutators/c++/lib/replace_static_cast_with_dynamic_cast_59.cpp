//source file
#include "../include/Replace_Static_Cast_With_Dynamic_Cast_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("StaticCast")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = Lexer::getSourceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = std::string(declaration);
      size_t pos = mutatedDeclaration.find("static_cast");
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, 11, "dynamic_cast");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cStyleCastExpr().bind("StaticCast");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}