//source file
#include "../include/Replace_Template_Parameter_With_Another_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = FT->getTemplateParameters();
      if (templateParams->size() < 2)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string originalCode = std::string(clang::Lexer::getSourceText(
          CharSourceRange::getTokenRange(FT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts()
      ).str());
      std::string mutatedCode = originalCode;
      // Replace the first template parameter with the second one
      std::string firstParamName = templateParams->getParam(0)->getNameAsString();
      std::string secondParamName = templateParams->getParam(1)->getNameAsString();
      size_t pos = mutatedCode.find(firstParamName);
      while (pos != std::string::npos) {
        mutatedCode.replace(pos, firstParamName.length(), secondParamName);
        pos = mutatedCode.find(firstParamName, pos + secondParamName.length());
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}