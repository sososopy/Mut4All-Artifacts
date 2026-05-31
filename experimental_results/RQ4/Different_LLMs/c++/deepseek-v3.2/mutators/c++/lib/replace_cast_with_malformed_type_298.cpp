//source file
#include "../include/Mutator_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CS = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CStyleCastExpr")) {
      //Filter nodes in header files
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto sourceRange = CS->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Generate malformed type
      std::string malformedType;
      int choice = getrandom::getRandomIndex(3);
      switch(choice) {
        case 0:
          malformedType = "invalid_type";
          break;
        case 1:
          malformedType = "int*int";
          break;
        case 2:
          malformedType = "void*void";
          break;
        case 3:
          malformedType = "double&int";
          break;
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Extract the source expression part
      std::string sourceExpr = stringutils::rangetoStr(*(Result.SourceManager), 
                                                       CS->getSubExpr()->getSourceRange());
      
      //Construct mutated cast expression
      std::string mutatedText = "(" + malformedType + ")" + sourceExpr;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cStyleCastExpr().bind("CStyleCastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}