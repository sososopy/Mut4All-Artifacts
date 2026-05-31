//source file
#include "../include/Mutator_Insert_Lambda_Capture_Default_416.h"

// ========================================================================================================
#define MUT416_OUTPUT 1

void MutatorFrontendAction_416::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      scopeVariables.push_back(VD);
    }
    else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = LE->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      //Check if lambda has explicit captures and no capture-default
      if (LE->getCaptureDefault() == clang::LCD_None && !LE->captures().empty()) {
        //Check if there are variables in scope
        if (scopeVariables.empty()) return;
        
        //Determine which capture-default to insert (= or &)
        int choice = getrandom::getRandomIndex(1);
        std::string captureDefault = (choice == 0) ? "=" : "&";
        
        //Find the capture list start position
        std::size_t captureStart = lambdaText.find('[');
        std::size_t captureEnd = lambdaText.find(']');
        if (captureStart == std::string::npos || captureEnd == std::string::npos) return;
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = lambdaText;
        mutatedText.insert(captureStart + 1, captureDefault + ", ");
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_416::MutatorASTConsumer_416::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("varDecl");
    StatementMatcher lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}