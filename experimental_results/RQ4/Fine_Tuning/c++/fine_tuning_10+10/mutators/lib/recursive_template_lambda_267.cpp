//source file
#include "../include/recursive_template_lambda_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (const auto *FuncDecl = FD->getTemplatedDecl()) {
        if (const auto *Body = FuncDecl->getBody()) {
          for (const auto &Stmt : Body->children()) {
            if (const auto *LambdaExpr = dyn_cast<clang::LambdaExpr>(Stmt)) {
              //Get the source code text of target node
              auto lambdaSource = Lexer::getSourceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
              //Perform mutation on the source code text by applying string replacement
              std::string newLambda = "myFunction([=]{});";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), newLambda);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(lambdaExpr(has(compoundStmt())))
    ).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}