//source file
#include "../include/Modify_Function_Return_Type_To_Invalid_One_194.h"

// ========================================================================================================
#define MUT194_OUTPUT 1

void MutatorFrontendAction_194::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto returntype =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getReturnTypeSourceRange());
      llvm::outs() << returntype << '\n';
      //Perform mutation on the source code text by applying string replacement
      returntype = "/*mut194*/NonExistentType";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getReturnTypeSourceRange(), returntype);
    }
}
  
void MutatorFrontendAction_194::MutatorASTConsumer_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}