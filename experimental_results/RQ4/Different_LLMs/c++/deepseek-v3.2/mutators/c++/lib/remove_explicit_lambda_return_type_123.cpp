//source file
#include "../include/Mutator_Remove_Explicit_Lambda_Return_Type_123.h"

// ========================================================================================================
#define MUT123_OUTPUT 1

void MutatorFrontendAction_123::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = MT->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Check if lambda has explicit return type
      auto method = MT->getCallOperator();
      if (!method->getReturnTypeSourceRange().isValid())
        return;
      
      //Get parameter list end location
      auto paramRange = MT->getIntroducerRange();
      SourceLocation paramEnd = paramRange.getEnd();
      
      //Find the arrow location
      SourceLocation arrowLoc = method->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
      if (arrowLoc.isInvalid())
        return;
      
      //Find the return type end location
      SourceLocation returnTypeEnd = method->getTypeSourceInfo()->getTypeLoc().getEndLoc();
      if (returnTypeEnd.isInvalid())
        return;
      
      //Get the source manager
      SourceManager &SM = *Result.SourceManager;
      
      //Calculate the range to remove (from arrow to end of return type)
      SourceLocation startRemoveLoc = arrowLoc;
      SourceLocation endRemoveLoc = returnTypeEnd;
      
      //Adjust end location to include any trailing whitespace before the body
      const char *sourceChars = SM.getCharacterData(endRemoveLoc);
      while (*sourceChars && (*sourceChars == ' ' || *sourceChars == '\t' || *sourceChars == '\n' || *sourceChars == '\r')) {
        endRemoveLoc = endRemoveLoc.getLocWithOffset(1);
        sourceChars = SM.getCharacterData(endRemoveLoc);
      }
      
      //Create the removal range
      CharSourceRange removeRange = CharSourceRange::getCharRange(startRemoveLoc, endRemoveLoc);
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the explicit return type with nothing (effectively removing it)
      //Replace the original AST node with the mutated one
      Rewrite.RemoveText(removeRange);
    }
}
  
void MutatorFrontendAction_123::MutatorASTConsumer_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}