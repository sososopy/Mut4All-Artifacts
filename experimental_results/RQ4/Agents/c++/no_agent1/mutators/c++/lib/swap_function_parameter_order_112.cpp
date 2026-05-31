//source file
#include "../include/swap_function_parameter_order_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getNumParams() < 2)
        return;

      //Get the source code text of target node
      auto param1 = FD->getParamDecl(0);
      auto param2 = FD->getParamDecl(1);
      auto param1Range = param1->getSourceRange();
      auto param2Range = param2->getSourceRange();

      auto param1Text = stringutils::rangetoStr(*(Result.SourceManager), param1Range);
      auto param2Text = stringutils::rangetoStr(*(Result.SourceManager), param2Range);

      //Perform mutation on the source code text by applying string replacement
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(param1Range), param2Text);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(param2Range), param1Text);
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, decl()), hasParameter(1, decl())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}