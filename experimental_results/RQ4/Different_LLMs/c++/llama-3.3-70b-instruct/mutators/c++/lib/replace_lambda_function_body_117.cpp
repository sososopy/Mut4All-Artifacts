//source file
#include "../include/Replace_Lambda_Function_Body_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 Lambda->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the body of the lambda function with a new one
      // that includes a requires clause with a different condition
      std::string mutatedDeclaration = declaration;
      size_t requiresPos = mutatedDeclaration.find("requires");
      if (requiresPos != std::string::npos) {
        size_t openBracePos = mutatedDeclaration.find("{", requiresPos);
        size_t closeBracePos = mutatedDeclaration.find("}", openBracePos);
        std::string requiresClause = mutatedDeclaration.substr(openBracePos + 1, closeBracePos - openBracePos - 1);
        // Replace the condition in the requires clause with a different one
        std::string newRequiresClause = "obj.empty(params...)";
        mutatedDeclaration.replace(openBracePos + 1, requiresClause.length(), newRequiresClause);
        // Replace the return value of the lambda function
        size_t returnPos = mutatedDeclaration.find("return");
        if (returnPos != std::string::npos) {
          size_t returnValPos = mutatedDeclaration.find(";", returnPos);
          std::string returnVal = mutatedDeclaration.substr(returnPos + 6, returnValPos - returnPos - 6);
          mutatedDeclaration.replace(returnPos + 6, returnVal.length(), "true");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}