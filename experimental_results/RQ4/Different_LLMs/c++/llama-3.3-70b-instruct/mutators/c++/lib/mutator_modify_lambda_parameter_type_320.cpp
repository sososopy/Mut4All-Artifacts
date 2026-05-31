//source file
#include "../include/Modify_Lambda_Parameter_Type_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), Lambda->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> paramTypes = {"int", "double", "float", "char"};
      int choice = getrandom::getRandomIndex(paramTypes.size() - 1);
      std::string newParamType = paramTypes[choice];
      size_t paramStart = lambdaText.find('(');
      size_t paramEnd = lambdaText.find(')');
      std::string paramDecl = lambdaText.substr(paramStart + 1, paramEnd - paramStart - 1);
      size_t spacePos = paramDecl.find(' ');
      if (spacePos != std::string::npos) {
        std::string paramName = paramDecl.substr(spacePos + 1);
        lambdaText.replace(paramStart + 2, spacePos - paramStart - 1, newParamType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}