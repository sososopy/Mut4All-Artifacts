//source file
#include "../include/remove_parenthesis_in_function_declaration_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      if (FD->getNumParams() > 0) {
        //Perform mutation on the source code text by applying string replacement
        int choice = getrandom::getRandomIndex(1);
        if (choice)
          decl.erase(decl.find('('), 1);
        else
          decl.erase(decl.rfind(')'), 1);
      } else {
        int choice = getrandom::getRandomIndex(1);
        if (choice)
          decl.erase(decl.find('('), 1);
        else
          decl.erase(decl.rfind(')'), 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}