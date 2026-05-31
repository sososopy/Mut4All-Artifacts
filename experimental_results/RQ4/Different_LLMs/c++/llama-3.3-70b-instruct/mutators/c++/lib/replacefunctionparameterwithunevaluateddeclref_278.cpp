//source file
#include "../include/ReplaceFunctionParameterWithUnevaluatedDeclRef_278.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto args = Call->arguments();
      if (args.begin() == args.end())
        return;
      //Perform mutation on the source code text by applying string replacement
      for (auto arg : args) {
        if (auto *Var = dyn_cast<clang::DeclRefExpr>(arg)) {
          if (auto *VarDecl = Var->getDecl()) {
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(arg->getSourceRange(), "&" + VarDecl->getNameAsString());
          }
        }
      }
    }
}
  
void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}