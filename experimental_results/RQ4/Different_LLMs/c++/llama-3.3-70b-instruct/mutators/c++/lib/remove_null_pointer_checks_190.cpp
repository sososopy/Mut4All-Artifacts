//source file
#include "../include/Mutator_Remove_Null_Pointer_Checks_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = MT->getCond();
      if (auto *binaryOp = dyn_cast<clang::BinaryOperator>(condition)) {
        if (binaryOp->getOpcode() == clang::BO_NE) {
          auto *rhs = binaryOp->getRHS();
          if (auto *nullPtr = dyn_cast<clang::IntegerLiteral>(rhs)) {
            if (nullPtr->getValue() == 0) {
              //Perform mutation on the source code text by applying string replacement
              Rewrite.RemoveText(CharSourceRange::getTokenRange(MT->getSourceRange()));
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::ifStmt(
        clang::ast_matchers::hasCondition(
            clang::ast_matchers::binaryOperator(
                clang::ast_matchers::hasOperatorName("!="),
                clang::ast_matchers::hasRHS(
                    clang::ast_matchers::integerLiteral(
                        clang::ast_matchers::equals(0)
                    )
                )
            )
        )
    ).bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}