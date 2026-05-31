//source file
#include "../include/Introduce_Callable_Template_Constructor_428.h"

// ========================================================================================================
#define MUT428_OUTPUT 1

void MutatorFrontendAction_428::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ConstructExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto constructorDecl = MT->getConstructor();
      if (!constructorDecl || !constructorDecl->getDeclName().isIdentifier())
        return;
      auto templateDecl = constructorDecl->getParent();
      if (!templateDecl || !templateDecl->isTemplateDecl())
        return;
      auto templateParams = dyn_cast<ClassTemplateSpecializationDecl>(templateDecl);
      if (!templateParams || templateParams->getTemplateArgs().size() == 0)
        return;
      auto numArgs = MT->getNumArgs();
      if (numArgs == 0)
        return;
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      string lambdaExpr = "[](){}";
      string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      size_t openParenPos = originalText.find('(');
      if (openParenPos == string::npos)
        return;
      size_t closeParenPos = originalText.find(')', openParenPos);
      if (closeParenPos == string::npos)
        return;
      string mutatedText = originalText.substr(0, openParenPos + 1) + lambdaExpr + originalText.substr(closeParenPos);
      mutatedText = "/*mut428*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_428::MutatorASTConsumer_428::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr().bind("ConstructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}