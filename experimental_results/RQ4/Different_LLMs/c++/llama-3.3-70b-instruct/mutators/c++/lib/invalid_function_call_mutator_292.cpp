//source file
#include "../include/Invalid_Function_Call_Mutator_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Modify the function call to pass an incorrect number or type of arguments
      std::string mutatedDeclaration = declaration;
      if (MT->getNumArgs() > 0) {
        // Remove the last argument
        size_t lastArgStart = declaration.find_last_of(',');
        if (lastArgStart != std::string::npos) {
          mutatedDeclaration = declaration.substr(0, lastArgStart);
        }
      } else {
        // Add an extra argument
        mutatedDeclaration += ", 0";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}