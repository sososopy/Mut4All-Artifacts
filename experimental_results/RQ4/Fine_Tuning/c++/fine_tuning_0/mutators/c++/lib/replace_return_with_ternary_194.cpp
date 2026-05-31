//source file
#include "../include/Replace_Return_With_Ternary_194.h"

// ========================================================================================================
#define MUT194_OUTPUT 1

void MutatorFrontendAction_194::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->hasBody())
        return;
      //Get the source code text of target node
      auto body = MT->getBody();
      auto return_stmt = body->children().begin();
      if (return_stmt->getStmtClass() != Stmt::ReturnStmtClass)
        return;
      auto return_expr = llvm::dyn_cast<ReturnStmt>(*return_stmt)->getRetValue();
      auto return_type = MT->getReturnType();
      //Perform mutation on the source code text by applying string replacement
      std::string mutated_return = "(" + std::string(return_expr->getStmtClassName()) +
                                   " > 0) ? " +
                                   std::string(return_expr->getStmtClassName()) + " : -" +
                                   std::string(return_expr->getStmtClassName());
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(return_expr->getSourceRange(), mutated_return);
    }
}
  
void MutatorFrontendAction_194::MutatorASTConsumer_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}