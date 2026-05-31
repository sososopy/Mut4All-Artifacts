//source file
#include "../include/replace_function_body_with_throw_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && !FD->isMain()) {
        //Get the source code text of target node
        auto bodyRange = FD->getBody()->getSourceRange();
        //Perform mutation on the source code text by applying string replacement
        std::string throwStmt = "{ throw std::runtime_error(\"mut390\"); }";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), throwStmt);
      }
    }
}
  
void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}