//source file
#include "../include/replace_function_body_with_recursive_call_364.h"

// ========================================================================================================
#define MUT364_OUTPUT 1

void MutatorFrontendAction_364::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && !FD->isMain()) {
        //Get the source code text of target node
        auto functionName = FD->getNameAsString();
        auto returnType = FD->getReturnType().getAsString();
        std::string recursiveCall = "{ /*mut364*/ return " + functionName + "(); }";
        
        //Perform mutation on the source code text by applying string replacement
        if (returnType != "void") {
          Rewrite.ReplaceText(FD->getBody()->getSourceRange(), recursiveCall);
        }
      }
    }
}
  
void MutatorFrontendAction_364::MutatorASTConsumer_364::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition(), unless(isMain())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}