//source file
#include "../include/Loop_Init_Modification_100.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ForLoop = Result.Nodes.getNodeAs<clang::ForStmt>("ForLoop")) {
      //Filter nodes in header files
      if (!ForLoop || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ForLoop->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto init = ForLoop->getInit();
      if (auto *Cond = dyn_cast<clang::ConditionalOperator>(init)) {
        // Swap the true and false branches of the condition
        auto trueExpr = Cond->getTrueExpr();
        auto falseExpr = Cond->getFalseExpr();
        std::string newCondStr = "(" + Cond->getCond()->getSourceRange().getBegin().printToString(*Result.SourceManager) + ") ? " + falseExpr->getSourceRange().getBegin().printToString(*Result.SourceManager) + " : " + trueExpr->getSourceRange().getBegin().printToString(*Result.SourceManager);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), newCondStr);
      } else if (auto *Expr = dyn_cast<clang::Expr>(init)) {
        // Change the assigned value to a different constant or a variable that exists in the scope
        auto varName = "x";
        std::string newInit = "/*mut100*/" + std::string(varName) + " = 0";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), newInit);
      } else if (auto *Stmt = dyn_cast<clang::Stmt>(init)) {
        // Replace the function call with a different function that exists in the code or with a constant value
        auto newInit = "/*mut100*/0";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()), newInit);
      }
    }
}
  
void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = forStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}