//source file
#include "../include/Modify_Function_Return_Type_To_Void_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (!FD->getReturnType()->isVoidType()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        auto returnTypeLoc = FD->getReturnTypeSourceRange();
        Rewrite.ReplaceText(returnTypeLoc, "void");

        // Remove return statements inside the function body
        if (auto *Body = FD->getBody()) {
          for (auto &Stmt : Body->children()) {
            if (auto *ReturnStmt = dyn_cast<clang::ReturnStmt>(Stmt)) {
              Rewrite.RemoveText(ReturnStmt->getSourceRange());
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(returns(voidType()))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}